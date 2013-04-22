#include "yocto/ios/rc.hpp"
#include "yocto/exception.hpp"
#include "yocto/fs/vfs.hpp"

#include <iostream>

using namespace yocto;

int main( int argc, char *argv[] )
{
    const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        std::cerr << "**** Opening Attached Resources" << std::endl;
        ios::resources rc(argv[0]);
        
        for( ios::resources::iterator i = rc.begin(); i != rc.end(); ++i )
        {
            std::cerr << "**** \t" << (*i).label << " : length=" << (*i).bytes << std::endl;
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
