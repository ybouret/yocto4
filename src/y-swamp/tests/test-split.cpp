
#include "yocto/utest/run.hpp"

#include "yocto/swamp/in1d.hpp"
#include "yocto/swamp/in2d.hpp"
#include "yocto/swamp/in3d.hpp"
#include "yocto/swamp/dataspace.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace swamp;

template <typename LAYOUT>
static inline void do_split( const LAYOUT &L )
{
    std::cerr << "main=" << L << std::endl;
    for(size_t size=1; size <= 5; ++size )
    {
        for( size_t rank=0; rank < size; ++rank )
        {
            const LAYOUT sub( L.split(rank,size) );
            std::cerr << "sub " << rank << "/" << size << "=" << sub << std::endl;
        }
        std::cerr << std::endl;
    }

}

YOCTO_UNIT_TEST_IMPL(split)
{
    
    {
        const layout1D L(1,100);
        do_split(L);
    }
    
    {
        const coord2D  lo(1,1);
        const coord2D  hi(10,100);
        const layout2D L(lo,hi);
        do_split(L);
    }

    
}
YOCTO_UNIT_TEST_DONE()
