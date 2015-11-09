#include "yocto/chemical/library.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(library)
{
    library lib;

    std::cerr << "Ready" << std::endl;
    
    string        line;
    ios::icstream input( ios::cstdin );
    xnode *node = lib.parse_species(input);
    auto_ptr<xnode> sp( node );
    if(sp.is_valid())
    {
        sp->graphviz("spec.dot");
        ios::graphviz_render("spec.dot");
        const xnode::leaves &ch = node->children();
        for(const xnode *sub = ch.head; sub; sub=sub->next)
        {
            if("molecule"==sub->label)
            {
                const string id = species::nameOf(sub);
                std::cerr << "id=" << id << std::endl;
                species::pointer sp( species::create(sub) );
                std::cerr << "sp=" << sp->name << std::endl;
                std::cerr << "charge=" << sp->charge << std::endl;
            }
        }
    }


}
YOCTO_UNIT_TEST_DONE()
