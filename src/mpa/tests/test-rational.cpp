#include "yocto/utest/run.hpp"
#include "yocto/mpa/rational.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(rational)
{
    mpq q;
    std::cerr << "q=" << q << std::endl;
    q = mpq(123,9);
    std::cerr << "q=" << q << std::endl;
    mpq p = mpq(-2,6);
    std::cerr << "p=" << p << std::endl;
    std::cerr << "p+q=" << p+q << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()

