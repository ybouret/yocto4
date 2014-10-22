#include "yocto/fame/visit.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/fame/mesh/rectilinear.hpp"

using namespace yocto;
using namespace fame;

namespace
{
    
    class MyData : public arrays
    {
    public:
        
        MyData()
        {
            this->store( new array1D<double>("X",layout1D(-5,5)) );
            this->store( new array1D<double>("Y",layout1D(-6,6)) );
            this->store( new array1D<double>("Z",layout1D(-7,7)) );
        }
        
        virtual ~MyData() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyData);
        
    };
    
    
    
    class MySim : public MyData, public VisIt::Simulation
    {
    public:
        
        explicit MySim( const mpi &ref) :
        MyData(),
        VisIt::Simulation( ref ),
        m2(*this,"mesh2d"),
        m3(*this,"mesh3d")
        {
            
        }
        
        virtual ~MySim() throw() {}
        
        //! sending meta data
        virtual void get_meta_data( visit_handle &md ) const
        {
            //
            add_generic_command("quit",md);
            
            // meshes
            visit::add_mesh_meta_data(md, m2);
            
            visit::add_mesh_meta_data(md, m3);
            
        }
        
        RectilinearMesh<2,double> m2;
        RectilinearMesh<3,double> m3;

        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MySim);
    };
    
    
}


YOCTO_UNIT_TEST_IMPL(fame)
{
    const string sim_name    = "VisitAndFame";
    const string sim_comment = "Visit I/O";
    const string sim_path    = ".";
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path,NULL);
    
    MySim sim(MPI);
    
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()
