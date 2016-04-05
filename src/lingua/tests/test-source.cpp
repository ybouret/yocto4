#include "yocto/lingua/source.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"


using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(source)
{

    ios::icstream fp( ios::cstdin );
    source        src(fp);
    token  tkn;
    t_char *ch;
    while( 0 != (ch=src.query()) )
    {
        tkn.push_back(ch);
    }

    const string r1 = tkn.to_string();

    // restoring
    while(tkn.size)
    {
        src.store(tkn.pop_back());
    }

    while( 0 != (ch=src.query()) )
    {
        tkn.push_back(ch);
    }

    const string r2 = tkn.to_string();
    if(r1!=r2)
    {
        throw exception("bad source.store!");
    }


    src.store(tkn); assert(0==tkn.size);
    while( 0 != (ch=src.query()) )
    {
        tkn.push_back(ch);
    }

    const string r3 = tkn.to_string();
    if(r1!=r3)
    {
        throw exception("bad source.store/token!");
    }

    src.unread(tkn); tkn.clear(); assert(0==tkn.size);
    while( 0 != (ch=src.query()) )
    {
        tkn.push_back(ch);
    }

    const string r4 = tkn.to_string();
    if(r1!=r4)
    {
        throw exception("bad source.unread");
    }


    std::cerr << "OK for source..." << std::endl;

}
YOCTO_UNIT_TEST_DONE()
