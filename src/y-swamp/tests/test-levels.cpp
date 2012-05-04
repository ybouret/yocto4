#include "yocto/utest/run.hpp"

#include "yocto/swamp/level-set.hpp"


using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(levels)
{
       
    level_set<double> levels;
    levels.add(0,0.1);
    levels.add(1,0.2);
    
    for( size_t i=1; i <= levels.size(); ++i )
    {
        std::cerr << levels[i].value << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()