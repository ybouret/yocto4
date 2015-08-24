#include "yocto/utest/run.hpp"
#include "yocto/container/slots.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(slots)
{
    slots_of<int> s(10);
    for(size_t i=1;i<=s.capacity/2;++i)
    {
        s.push_back(i);
    }

    for(size_t i=1;i<=s.size;++i)
    {
        std::cerr << "s[" << i << "]=" << s[i] << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()
