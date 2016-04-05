#include "yocto/lingua/token.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(token)
{
    token tkn = "Hello, World!";
    std::cerr << tkn << std::endl;
    token t2 = tkn;
    std::cerr << t2 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
