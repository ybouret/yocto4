#include "yocto/gfx/named-colors.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(named)
{
    std::cerr << "#named_colors=" << named_color::num << std::endl;
    for(size_t i=0;i<named_color::num;++i)
    {
        const char *id = named_color::reg[i].name;
        const rgb_t C = named_color::get(id);
        std::cerr << id << ": " << int(C.r) << " " << int(C.g) << " " << int(C.b) << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
