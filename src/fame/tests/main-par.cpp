#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/mpi/mpi.hpp"


using namespace yocto;
using namespace fame;


int main(int argc, char *argv[])
{
    
    bool pbc[3] = { true, true, true };
    
    try
    {
        YOCTO_MPI_ENV();
        
        //______________________________________________________________________
        //
        // in 1 D
        //______________________________________________________________________
        {
            
        }
        
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught Exception!" << std::endl;
    }
    return 1;
}