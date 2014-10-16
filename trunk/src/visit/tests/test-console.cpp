
#include "yocto/visit/visit.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(console)
{
    
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,"TestConsole", "Testing Console...", ".");
    
    VisIt::Simulation sim(MPI);
    
    
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()

