#include "./module.hpp"
#include "yocto/rtld/interface.hpp"

#include "yocto/exception.hpp"
#include <iostream>

using namespace yocto;

int main( int argc, char *argv[] )
{

    try
    {
        std::cerr << "sizeof(rtld_content)="   << sizeof(rtld_content)   << std::endl;
        std::cerr << "sizeof(interface<ops>)=" << sizeof(interface<ops>) << std::endl;
        if( argc < 2 )
            throw exception("Usage: loader module.dll");
        const string soname = argv[1];

        if(false)
        {
            std::cerr << "-- Loading " << soname << std::endl;
            dylib *dll = dylib_load( soname.c_str(), dylib_full );
            const module  m( dll );
            std::cerr << "loaded <" << soname << ">" << std::endl;

            std::cerr << "-- Linking 'sum'" << std::endl;
            int (*sum)(int a, int b)  = 0;
            m.link( sum, "sum" );
            if( !sum )
                throw exception("Can't find 'sum'");
            std::cerr << "sum@" << (void*)sum << std::endl;
            std::cerr << "-- Executing function..." << std::endl;
            std::cerr << sum(1,2) << std::endl;

            std::cerr << "-- Loading it a second time" << std::endl;
            const module m2( dll );
        }

        std::cerr << "loader name: " << rtld_content::loader_name << std::endl;
        std::cerr << "-- Loading plugin" << std::endl; std::cerr.flush();
        interface<ops> I(soname);
        std::cerr << "#entries=" << I.num_entries << std::endl;
        
        if(I.is_loaded())
        {
            std::cerr << "-- Plugin is Loaded" << std::endl;
        }
        else
        {
            std::cerr << "-- Plugin is not loaded: " << I.when() << std::endl;
            return 0;
        }
        std::cerr.flush();

        assert(I->add);
        assert(I->mul);


        std::cerr << "add: " << I->add(2,3) << std::endl;
        std::cerr << "mul: " << I->mul(2,3) << std::endl;



        std::cerr << "-- Done" << std::endl;
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
