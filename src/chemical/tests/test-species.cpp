#include "yocto/chemical/library.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/imstream.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(species)
{

    library lib;

    for(int i=1;i<argc;++i)
    {
        const string    m( argv[i] );
        std::cerr << "input=" << m << std::endl;
        ios::imstream   fp(m);
        xnode          *wrapper = lib.parse_species(fp);
        if(wrapper)
        {
            auto_ptr<xnode> guard(wrapper);
            const xnode *molecule = wrapper->head();
            const string    id = species::nameOf(molecule);
            std::cerr << "found name=" << id << std::endl;
            species::pointer sp( species::create(molecule) );

            std::cerr << "sp.name    = " << sp->name    << std::endl;
            std::cerr << "sp.charge  = " << sp->charge  << std::endl;
            std::cerr << "sp.formula = " << sp->formula << std::endl;
        }
    }

}
YOCTO_UNIT_TEST_DONE()
