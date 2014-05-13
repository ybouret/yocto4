#include "yocto/utest/run.hpp"
#include "yocto/mpa/rational.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(rational)
{
    mpq q;
    std::cerr << "q=" << q << " approx " << q.to_double() << std::endl;
    q = mpq(123,9);
    std::cerr << "q=" << q << " approx " << q.to_double() << std::endl;
    mpq p = mpq(-2,6);
    std::cerr << "p=" << p << " approx " << p.to_double() << std::endl;
    std::cerr << "p+q=" << p+q << std::endl;
    
    q = 1;
    std::cerr << "q=" << q << " => " << q.to_double() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

