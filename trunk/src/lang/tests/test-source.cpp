#include "yocto/lang/source.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(source)
{
    ios::icstream fp( ios::cstdin );
    lang::token   tkn;
    lang::t_char *ch = 0;
    lang::source  src;
    {
        lang::token pre = "<SOURCE>";
        src.unget(pre);
        const lang::token pre2 = "TKN:";
        src.uncpy(pre2);
    }
    while( NULL != (ch=src.get(fp)) )
        tkn.push_back(ch);
    std::cerr << "tkn=" << tkn << std::endl;
}
YOCTO_UNIT_TEST_DONE()
