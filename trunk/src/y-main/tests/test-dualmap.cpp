#include "yocto/associative/dualmap.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(dualmap)
{
    dualmap<int, string, float> dm;
    
    const string s = "toto";
    
    (void) dm.get_key_of(s);
    (void) dm.get_sub_of(10);
    
    dualmap<int, string, float> dm2(10,as_capacity);
    
    dm2.insert(10, s, 1.0f);
    dm2.insert(11, "titi", 2.0f);
    std::cerr << "dm2.size=" << dm2.size() << std::endl;
    dm.swap_with(dm2);
    std::cerr << "dm2.size=" << dm2.size() << std::endl;
    
    std::cerr << "dm.size="  << dm.size() << std::endl;
    std::cerr << "dm.capacity=" << dm.capacity() << std::endl;
    dm.reserve(4);
    std::cerr << "dm.size="  << dm.size() << std::endl;
    std::cerr << "dm.capacity=" << dm.capacity() << std::endl;
    
    for(size_t iter=10;iter>0;--iter)
    {
        for(size_t i=10 + alea_leq(1000); i>0;--i)
        {
            const int    k = gen<int>::get();
            const string s = gen<string>::get();
            const float  f = gen<float>::get();
            
            (void) dm.insert(k,s,f);
        }
        std::cerr << "dm.size="     << dm.size()     << std::endl;
        std::cerr << "dm.capacity=" << dm.capacity() << std::endl;
        dm.release();
    }
    
    for(size_t i=10 + alea_leq(10); i>0;--i)
    {
        const int    k = gen<int>::get();
        const string s = gen<string>::get();
        const float  f = gen<float>::get();
        
        if( dm.insert(k,s,f) )
        {
            std::cerr << "inserted (" << k << "," << s << ")=" << f << std::endl;
        }
    }
    std::cerr << std::endl;
    for( dualmap<int, string, float>::iterator i = dm.begin(); i != dm.end(); ++i)
    {
        std::cerr << "@(" << i->key << "," << i->sub << ") : " << *i << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
