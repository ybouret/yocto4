#include "yocto/utest/run.hpp"
#include "yocto/lingua/token.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(token)
{
    token t;
    
    t = "token";
    std::cerr << "token=<" << t << ">" << std::endl;
    
    {
        const string s = "token2";
        t = s;
    }
    std::cerr << "token=<" << t << ">" << std::endl;
    
    {
        token t2(t);
        std::cerr << "token=<" << t2 << ">" << std::endl;
        t.merge_back(t2);
        std::cerr << "token=<" << t2 << ">" << std::endl;
    }
    std::cerr << "token=<" << t << ">" << std::endl;
    
    t = "hello";
    std::cerr << t.to_string() << std::endl;
    std::cerr << t.to_string(1,0) << std::endl;
    std::cerr << t.to_string(0,1) << std::endl;
    std::cerr << t.to_string(1,1) << std::endl;
    std::cerr << t.to_string(2,2) << std::endl;
    std::cerr << t.to_string(3,3) << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
