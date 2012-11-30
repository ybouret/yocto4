#include "./module.hpp"
#include "yocto/rtld/module.hpp"
#include "yocto/exception.hpp"
#include <iostream>

using namespace yocto;

int main( int argc, char *argv[] )
{
    
    try
    {
        if( argc < 2 )
            throw exception("Usage: loader module.dll");
        const string soname = argv[1];
        module  m( soname );
        std::cerr << "loaded <" << soname << ">" << std::endl;

        int (*sum)(int a, int b)  = 0;
        m.link( sum, "sum" );
        std::cerr << "sum@" << (void*)sum << std::endl;
        std::cerr << sum(1,2) << std::endl;
        
        return 0;
    }
    catch(const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception" << std::endl;
    }
    return 1;
}