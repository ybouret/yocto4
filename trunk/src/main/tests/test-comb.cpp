#include "yocto/utest/run.hpp"
#include "yocto/code/combination.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(comb)
{
        
    
    size_t n = (argc > 1) ? strconv::to<size_t>(argv[1],"n") : 10 ;
    size_t k = (argc > 2) ? strconv::to<size_t>(argv[2],"k") :  3 ;
    combination C(n,k);
    
    size_t count = 0;
    do
    {
        ++count;
        std::cerr << C << std::endl;
    }
    while( C.next() );
    
    std::cerr << "C(" << n << "," << k << ")=" << count << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
