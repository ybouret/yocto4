#include "yocto/utest/run.hpp"
#include "./main.hpp"

template <typename LAYOUT>
static inline
void do_split( const LAYOUT &L, offsets_list &offsets )
{
    std::cerr << "Splitting for dimension=" << L.dimensions << std::endl;
    for( size_t size = 1; size <= 3; ++size )
    {
        try
        {
            for( size_t rank=0;rank<size;++rank)
            {
                const LAYOUT sub = L.split(rank,size);
                std::cerr << "\t " << size << "." << rank << ": " << sub << std::endl;
                offsets.free();
                L.load_offsets( sub, offsets );
            }
        }
        catch (const exception &e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << e.when() << std::endl;
        }
        
    }
    std::cerr << std::endl;
    
}

YOCTO_UNIT_TEST_IMPL(layout)
{
    
    offsets_list offsets;
    for( size_t i=0; i < 4; ++i )
    {
        layout1D L1( RandCoord1D(), RandCoord1D());
        std::cerr << "L1=" << L1 << std::endl;
        do_split(L1,offsets);
        
        layout2D L2( RandCoord2D(), RandCoord2D() );
        std::cerr << "L2=" << L2 << std::endl;
        do_split(L2,offsets);

        layout3D L3( RandCoord3D(), RandCoord3D() );
        std::cerr << "L3=" << L3 << std::endl;
        do_split(L3,offsets);

    }
}
YOCTO_UNIT_TEST_DONE()
