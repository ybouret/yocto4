#include "yocto/utest/run.hpp"
#include "yocto/seem/compiler.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(compiler)
{
    Seem::Compiler cmp(true);
    cmp.parser.gram->display();
    
    ios::icstream  fp( ios::cstdin );
    cmp.compile(fp);

}
YOCTO_UNIT_TEST_DONE()

