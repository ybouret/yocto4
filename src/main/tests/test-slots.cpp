#include "yocto/utest/run.hpp"
#include "yocto/sequence/slots.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(slots)
{
    std::cerr << "Testing POD" << std::endl;
    {
        slots_of<int> s(10);
        for(size_t i=1;i<=s.capacity/2;++i)
        {
            s.push_back(i);
        }

        for(size_t i=s.size+1;i<=s.capacity;++i)
        {
            s.append<size_t>(alea_lt(100));
        }

        for(size_t i=0;i<s.size;++i)
        {
            std::cerr << "s[" << i << "]=" << s[i] << std::endl;
        }
    }
    std::cerr << std::endl;



    std::cerr << "Testing C++" << std::endl;
    {
        slots_of<string> s(19);
        for(size_t i=1;i<=s.capacity;++i)
        {
            const string tmp = gen<string>::get();
            s.push_back(tmp);
        }
        for(size_t i=0;i<s.size;++i)
        {
            std::cerr << "s[" << i << "]=" << s[i] << std::endl;
        }
    }
    std::cerr << std::endl;


    std::cerr << "Testing Helpers..." << std::endl;
    {
        slots_of<double> tgt(4);
        slots_of<int>    src(8);
        while(src.size<src.capacity)
        {
            src.push_back( alea_lt(100) );
        }
        tgt.build_from(src);
        for(size_t i=0;i<tgt.size;++i)
        {
            std::cerr << "#" << i << "\t" << src[i] << " -> \t" << tgt[i] << std::endl;
        }
        std::cerr << "tgt=" << tgt << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/slots2d.hpp"

YOCTO_UNIT_TEST_IMPL(slots2D)
{

    slots2D_of<double>   sd(3,4);
    sd.build();

    slots2D_of<int16_t>  sw(3,4);
    sw.build_with<int>(10);

    std::cerr << "sw=" << sw << std::endl;

    slots2D_of<string> S(5,6);
    S.build();
    for(size_t i=0;i<S.items;++i) S(i) = gen<string>::get();
    std::cerr << "S=" << S << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
