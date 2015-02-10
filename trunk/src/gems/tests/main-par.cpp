#include "yocto/gems/mpi.hpp"

using namespace yocto;
using namespace gems;

int main(int argc, char *argv[])
{
    try
    {
        YOCTO_MPI_ENV();
        MPI.CloseStdIO();

        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Something Bad Happened!" << std::endl;
    }
    return 1;
    
}
