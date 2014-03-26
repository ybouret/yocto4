#include "yocto/utest/run.hpp"
#include "yocto/mpa/integer.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(int)
{
    mpz x = -89;
    std::cerr << "x=" << x << std::endl;
}
YOCTO_UNIT_TEST_DONE()
