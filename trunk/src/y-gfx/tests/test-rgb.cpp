#include "yocto/utest/run.hpp"
#include "yocto/gfx/named-rgb.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(rgb)
{
    vector<string> names;
    gfx::get_named_rgb(names);
    for(size_t i=1;i<=names.size();++i)
    {
        std::cerr << names[i];
        const gfx::rgb_t c = gfx::get_named_rgb(names[i]);
        std::cerr << " " << int(c.r) << " " << int(c.g) <<  " " << int(c.b);
        std::cerr << std::endl;
    }
    std::cerr << "#names=" << names.size() << std::endl;
    //names.release();
    gfx::rgb_t red = "red";
}
YOCTO_UNIT_TEST_DONE()
