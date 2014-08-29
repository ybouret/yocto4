#include "yocto/lang/token.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(token)
{
    std::cerr << "sizeof(lang::t_char)=" << sizeof(lang::t_char) << std::endl;
    std::cerr << "sizeof(lang::token) =" << sizeof(lang::token)  << std::endl;

    lang::token t1 = "Hello";
    std::cerr << "t1=" << t1 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
