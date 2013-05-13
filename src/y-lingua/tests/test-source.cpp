#include "yocto/utest/run.hpp"
#include "yocto/lingua/source.hpp"
#include "yocto/ios/imstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(source)
{
    t_cache p;
    source  src(p);
    
    token t(p);
    token r(p);
    
    t = ", world!";
    src.unget(t);
    t = "hello";
    src.uncpy(t);
    t.clear();
    
    const char data[] =" and hello again";
    {
        const istream_ptr fp( new ios::imstream(data,sizeof(data)));
        src.push("data", fp);
    }
    
    t_char *ch = 0;
    while( 0 != (ch=src.get()) )
    {
        r.push_back(ch);
    }
    
    std::cerr << "r=<" << r << ">" << std::endl;
    r.clear();
    t.clear();
    
    std::cerr << "cache size=" << p.size << std::endl;
}
YOCTO_UNIT_TEST_DONE()
