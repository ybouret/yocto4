#include "yocto/fame/visit.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

namespace
{
    
    
    
    class MySim : public VisIt::Simulation
    {
    public:
        
        explicit MySim() :
        VisIt::Simulation( *mpi::location() )
        {
            
        }
        
        virtual ~MySim() throw() {}
        
        //! sending meta data
        virtual void get_meta_data( visit_handle &md ) const
        {
            add_generic_command("quit",md);
        }
        
        
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
    
    MySim sim;
    
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()
