#include "yocto/utest/run.hpp"
#include "yocto/sort/remove-if.hpp"
#include "yocto/sequence/vector.hpp"


#include "support.hpp"

using namespace yocto;


static inline bool is_bad_int( int x )
{
    return x > 3;
}

static inline bool is_bad_str( const string &s )
{
    return s.size() <= 0 || s[0] == 'A';
}

YOCTO_UNIT_TEST_IMPL(remove_if)
{
    vector<int> a;
    
    for( size_t i= 2 + alea_leq(20); i>0; --i )
    {
        a.push_back( int(alea_leq(15) ) );
    }
    
    std::cerr << "a1=" << a << std::endl;
    remove_if(a,is_bad_int);
    std::cerr << "a2=" << a << std::endl;
    
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
    std::cerr << "b1=" << b << std::endl;
    remove_if(b,is_bad_str);
    std::cerr << "b2=" << b << std::endl;

}
YOCTO_UNIT_TEST_DONE()

