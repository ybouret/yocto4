#include "yocto/utest/run.hpp"
#include "yocto/sort/unique.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/comparator.hpp"
#include "yocto/code/rand.hpp"

#include "support.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(unique)
{
    vector<int> a;
    
    for( size_t i= 2 + alea_leq(20); i>0; --i )
    {
        a.push_back( int(alea_leq(15) ) );
    }
    
    std::cerr << "a1=" << a << std::endl;
    quicksort(a,__compare<int>);
    std::cerr << "a2=" << a << std::endl;
    unique(a, __compare<int> );
    std::cerr << "a3=" << a << std::endl;
    
    vector<string> b;
    for(size_t i=2+alea_leq(20);i>0;--i)
    {
        string s;
        for(size_t j=1+alea_lt(3);j>0;--j)
        {
            s += char( 'A' + alea_lt(2) );
        }
        b.push_back(s);
    }
    std::cerr << "b=" << b << std::endl;
    unique(b, __compare<string> );
    std::cerr << "b=" << b << std::endl;
}
YOCTO_UNIT_TEST_DONE()
