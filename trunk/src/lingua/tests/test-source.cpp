#include "yocto/utest/run.hpp"
#include "yocto/lingua/source.hpp"
#include "yocto/ios/imstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(source)
{
    source  src;
    
    token t;
    token r;
    
    t = ", world!";
    src.unget(t);
    t = "hello";
    src.uncpy(t);
    t.clear();
    
    const char data[] =" and hello again";
    {
        const input fp( new ios::imstream(data,sizeof(data)));
        src.attach(fp);
    }
    
    t_char *ch = 0;
    while( 0 != (ch=src.get()) )
    {
        r.push_back(ch);
    }
    
    std::cerr << "r=<" << r << ">" << std::endl;
    r.clear();
    t.clear();
    
}
YOCTO_UNIT_TEST_DONE()
