#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/lang/generator.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;

namespace
{

}

YOCTO_UNIT_TEST_IMPL(gen)
{
    generator G("dummy");

    ios::icstream fp( ios::cstdin );

    auto_ptr<syntax::xnode> tree( G.run(fp) );
    
}
YOCTO_UNIT_TEST_DONE()
