#include "yocto/lang/tchar-source.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(source)
{
    ios::icstream fp(ios::cstdin);

    lang::t_char_source src;
    lang::token tkn;
    lang::t_char *ch = 0;
    while( (ch=src.get(fp)) != 0 )
    {
        tkn.push_back(ch);
    }
    if(tkn.size>0) delete tkn.pop_back();
    std::cerr << "tkn=[" << tkn << "]" << std::endl;

    src.unget("/EOF");
    src.uncpy(tkn);
    tkn.clear();

    while( (ch=src.get(fp)) != 0 )
    {
        tkn.push_back(ch);
    }
    std::cerr << "tkn=[" << tkn << "]" << std::endl;



}
YOCTO_UNIT_TEST_DONE()
