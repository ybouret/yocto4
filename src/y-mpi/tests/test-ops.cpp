#include "yocto/utest/run.hpp"
#include "yocto/mpi/ops.hpp"

using namespace yocto;

#define __SHOW(TYPE) MPI.Printf(stderr, "%-16s : 0x%08x\n", #TYPE, unsigned(MPI.get_type<TYPE>()) )


template <typename T>
void __check_ops(const mpi &MPI, const char *fmt)
{
    const T x( MPI.CommWorldRank+1 );
    MPI.Printf(stderr, fmt, x);
    const T s = mpi_ops::sum(MPI,x);
    MPI.Printf0(stderr,"sum:\n");
    MPI.Printf(stderr,fmt,s);
    
    const T p = mpi_ops::prod(MPI, x);
    MPI.Printf0(stderr,"prod:\n");
    MPI.Printf(stderr,fmt,p);
    
    const T xmin =  mpi_ops::min(MPI, x);
    MPI.Printf0(stderr,"min:\n");
    MPI.Printf(stderr,fmt,xmin);
    
    const T xmax =  mpi_ops::max(MPI, x);
    MPI.Printf0(stderr,"max:\n");
    MPI.Printf(stderr,fmt,xmax);
}


YOCTO_UNIT_TEST_IMPL(ops)
{
	
    YOCTO_MPI;
    MPI.CloseStdIO();
 
    MPI.Printf0(stderr, "Checking DataBase\n");
    const array<mpi::db_item> &db = MPI.db;
    for( size_t i=1; i <= db.size(); ++i )
    {
        const mpi::db_item &item = db[i];
        MPI.Printf(stderr, "%-10s => 0x%08x\n", item.spec.name(), unsigned(item.id) );
        const MPI_Datatype res = MPI.get_type(item.spec);
        MPI.Printf(stderr,"found 0x%08x\n", unsigned(res));
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
    MPI.Printf0(stderr,"\n");

    MPI.Printf0(stderr, "Checking Ops\n");
    
    __check_ops<int>(MPI,      "int      : %d\n");
    __check_ops<float>(MPI,    "float    : %f\n");
    __check_ops<double>(MPI,   "double   : %lf\n");
    __check_ops<unsigned>(MPI, "unsigned : %u\n");

}
YOCTO_UNIT_TEST_DONE()
