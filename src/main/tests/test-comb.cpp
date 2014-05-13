#include "yocto/utest/run.hpp"
#include "yocto/code/combination.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(comb)
{
    
    static const size_t nmax = 1024;
    
    
    size_t n = (argc > 1) ? strconv::to<size_t>(argv[1],"n") : 10 ;
    size_t k = (argc > 2) ? strconv::to<size_t>(argv[2],"k") :  3 ;
    if(n>nmax) n = nmax;
    if(k>n)    k = n;
    
    size_t  count = 0;
    size_t *comb  = (size_t *)malloc(k*sizeof(size_t));
    if(!comb)
        throw exception("Out of Memory");
    
    combination::init(comb,k,n);
    do
    {
        ++count;
    }
    while( combination::next(comb,k,n) );
    free(comb);
    
    std::cerr << "C(" << n << "," << k << ")=" << count << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
