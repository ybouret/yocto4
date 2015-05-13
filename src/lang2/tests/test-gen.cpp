#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/lang/generator.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;

namespace
{

}

YOCTO_UNIT_TEST_IMPL(gen)
{
    generator G("dummy");
    vfs &fs = local_fs::instance();
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");


    ios::icstream           fp( ios::cstdin );
    auto_ptr<syntax::xnode> tree( G.compile(fp) );

   
}
YOCTO_UNIT_TEST_DONE()
