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
    
    auto_ptr<syntax::xnode> root( compiler::ast(cc(src)) );
    root->graphviz("tree.dot");
    system("dot -Tpng -otree.png tree.dot");
    
}
YOCTO_UNIT_TEST_DONE()
