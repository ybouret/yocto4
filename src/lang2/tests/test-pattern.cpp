#include "yocto/lang/pattern/basic.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pattern)
{
    ios::icstream fp(ios::cstdin);

    lang::source  src;
    lang::token   tkn;
    auto_ptr<lang::pattern> p( lang::any1::create() );

    p.reset( lang::single::create('a') );

    while( src.peek(fp) )
    {
        if(p->match(tkn, src, fp))
        {
            std::cerr << "<" << tkn << ">";
            tkn.clear();
        }
        else
        {
            assert(src.peek(fp));
            std::cerr << char(src.peek(fp)->code);
            src.skip(1);
        }
    }

}
YOCTO_UNIT_TEST_DONE()
