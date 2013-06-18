#include "yocto/utest/run.hpp"
#include "yocto/code/unique.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/comparator.hpp"
#include "yocto/code/rand.hpp"

#include "support.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(unique)
{
    vector<int> a;
    
    for( size_t i= 2 + alea_leq(10); i>0; --i )
    {
        a.push_back( int(alea_leq(10) ) );
    }
    
    std::cerr << "a1=" << a << std::endl;
    hsort(a,__compare<int>);
    std::cerr << "a2=" << a << std::endl;
    unique(a, __compare<int> );
    std::cerr << "a3=" << a << std::endl;

}
YOCTO_UNIT_TEST_DONE()
