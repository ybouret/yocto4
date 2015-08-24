#include "yocto/utest/run.hpp"
#include "yocto/container/slots.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(slots)
{
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

        for(size_t i=1;i<=s.size;++i)
        {
            std::cerr << "s[" << i << "]=" << s[i] << std::endl;
        }
    }


    {
        slots_of<string> s(19);
        for(size_t i=1;i<=s.capacity;++i)
        {
            const string tmp = gen<string>::get();
            s.push_back(tmp);
        }
        for(size_t i=1;i<=s.size;++i)
        {
            std::cerr << "s[" << i << "]=" << s[i] << std::endl;
        }
    }


}
YOCTO_UNIT_TEST_DONE()
