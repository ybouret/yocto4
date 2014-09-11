#include "yocto/lang/generator.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;


namespace
{
    
    
    
}

YOCTO_UNIT_TEST_IMPL(gen)
{
    
    vfs &fs = local_fs::instance();
    
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");
    
    
    generator     G;
    ios::icstream fp( ios::cstdin );
    
    auto_ptr<syntax::xnode> Tree( G.run(fp) );
    if(Tree.is_valid())
    {
        Tree->graphviz("xnode.dot");
        system("dot -Tpng -o xnode.png xnode.dot");
    }

}
YOCTO_UNIT_TEST_DONE()

