#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"

using namespace yocto;

#define __SHOW(TYPE) MPI.Printf(stderr, "%-16s : 0x%lx\n", #TYPE, ptrdiff_t(MPI.get_type<TYPE>()) )


template <typename T>
void __check_ops(const mpi &MPI, const char *fmt)
{
	const int v = MPI.CommWorldRank+1;
    const T   x = T(v);
	MPI.Printf0(stderr,"Checking ops with\n");
    MPI.Printf(stderr, fmt, x);
    const T s = MPI.Sum(x,MPI_COMM_WORLD);
    MPI.Printf0(stderr,"sum:\n");
    MPI.Printf(stderr,fmt,s);
    
    const T p = MPI.Prod(x,MPI_COMM_WORLD);
    MPI.Printf0(stderr,"prod:\n");
    MPI.Printf(stderr,fmt,p);
    
    const T xmin =  MPI.Min(x, MPI_COMM_WORLD);
    MPI.Printf0(stderr,"min:\n");
    MPI.Printf(stderr,fmt,xmin);
    
    const T xmax =  MPI.Max(x, MPI_COMM_WORLD);
    MPI.Printf0(stderr,"max:\n");
    MPI.Printf(stderr,fmt,xmax);

	MPI.Printf0(stderr,"\n");
}


YOCTO_UNIT_TEST_IMPL(ops)
{
	
    YOCTO_MPI(SINGLE);
    MPI.CloseStdIO();
 
    MPI.Printf0(stderr, "Checking DataBase, #items=%u\n", unsigned( MPI.db.size() ) );
    const array<mpi::data_type> &db = MPI.db;
    for( size_t i=1; i <= db.size(); ++i )
    {
        const mpi::data_type &item = db[i];
        MPI.Printf(stderr, "%-10s => 0x%lx\n", item.name(), ptrdiff_t(item.id) );
        const MPI_Datatype res = MPI.get_type(item);
        MPI.Printf(stderr,"found 0x%lx\n", ptrdiff_t(res));
    }
    MPI.Printf0(stderr,"\n");
    
    MPI.Printf0(stderr, "Checking Named Types\n");
    __SHOW(float);
    __SHOW(double);
    __SHOW(int);
    __SHOW(unsigned);
    __SHOW(size_t);
    __SHOW(long);
    __SHOW(unsigned long);
    __SHOW(ptrdiff_t);
    MPI.Printf0(stderr,"\n");

    
    MPI.Printf0(stderr, "Checking Ops\n");
    __check_ops<int>(MPI,      "int      : %d\n");
    __check_ops<float>(MPI,    "float    : %f\n");
    __check_ops<double>(MPI,   "double   : %lf\n");
    __check_ops<unsigned>(MPI, "unsigned : %u\n");
	
}
YOCTO_UNIT_TEST_DONE()
