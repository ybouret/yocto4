#include "yocto/ios/rc.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/vfs-utils.hpp"
#include "yocto/exception.hpp"

#include <iostream>

using namespace yocto;



int main( int argc, char *argv[] )
{
    const char *progname = _vfs::get_base_name(argv[0]);
    try
    {
        if( argc <= 1 )
            throw exception("usage: %s executable [resources]", progname);
        const string exename  = argv[1];
        error_type   status   = 0;
        std::cerr << "**** Packing resources in [" << exename << "]" << std::endl;
        
        {
            ios::resources::packer rc( exename, status);
        }
        
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception in " << progname << std::endl;
    }
    return -1;
}
