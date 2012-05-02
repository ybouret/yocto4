
#include "yocto/utest/run.hpp"

#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/code/rand.hpp"

#include <iostream>

using namespace yocto;
using namespace swamp;

template <typename LAYOUT>
static inline void display( const LAYOUT &L )
{
    std::cerr << "DIM= " << LAYOUT::DIMENSIONS << std::endl;
    std::cerr << L.lower << " -> " << L.upper << " = " << L.width << " | #=" << L.items << std::endl;
}

static inline coord1D rand1()
{
    return 50 - 100 * alea<double>();
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3()
{
    return coord3D( rand1(), rand1(), rand1() );
}

YOCTO_UNIT_TEST_IMPL(wksp)
{
    {
        const layout1D        L( rand1()-2, rand1()+2 );
        ghosts_setup<coord1D> G;
        G.local.count = 1; // +1 and -1
        
        workspace<layout1D> W(L,G);
        std::cerr << "W.Layout: ";
        display(W);
        std::cerr << "W.outline: ";
        display(W.outline);
    }
    
    
    {
        const coord2D         lo(1,1);
        const coord2D         hi(10,10);
        const layout2D        L(lo,hi);
        ghosts_setup<coord2D> G;
        G.local.count.y = 3;
        
        G.lower.count.x = 2;
        G.upper.count.x = 1;
        
        workspace<layout2D>   W(L,G);
        std::cerr << "W.Layout: ";
        display(W);
        std::cerr << "W.outline: ";
        display(W.outline);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()