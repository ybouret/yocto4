#include "yocto/utest/run.hpp"
#include "yocto/lingua/compiler.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lingua;


YOCTO_UNIT_TEST_IMPL(cc)
{
    vfs &fs = local_fs::instance();
    if(fs.is_reg("tree.dot" )) fs.remove_file("tree.dot");
    if(fs.is_reg("tree.png"))  fs.remove_file("tree.png");
    
    compiler cc;
    source   src;
    src.attach_stdin();
    
    auto_ptr<syntax::xnode> root( cc.run(src) );
    root->graphviz("tree.dot");
    int res=system("dot -Tpng -otree.png tree.dot");(void)res;
    cc.emit( root.__get() );
    
}
YOCTO_UNIT_TEST_DONE()
