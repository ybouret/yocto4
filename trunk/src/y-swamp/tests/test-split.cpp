
#include "yocto/utest/run.hpp"

#include "yocto/swamp/array3d.hpp"

#include "yocto/swamp/dataspace.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

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
        fields_setup<layout1D> F;
        Y_SWAMP_DECL_VAR(F, "A", array1D<double> );
        Y_SWAMP_DECL_VAR(F, "B", array1D<double> );
        Y_SWAMP_DECL_AUX(F, "C", array1D<double> );
        
        const layout1D L(1,100);
        do_split(L);
        const ghosts_setup<coord1D> no_ghosts;
        dataspace<layout1D> D(L,no_ghosts,F);
        
        const layout1D L1( L.split(0,3) );
        const layout1D L2( L.split(1,3) );
        const layout1D L3( L.split(2,3) );
        
        
        dataspace<layout1D> D1(L1,no_ghosts,F);
        dataspace<layout1D> D2(L2,no_ghosts,F);
        dataspace<layout1D> D3(L3,no_ghosts,F);

        array1D<double> &A  = D["A"].as< array1D<double> >();
        array1D<double> &A1 = D1["A"].as< array1D<double> >();
        array1D<double> &A2 = D2["A"].as< array1D<double> >();
        array1D<double> &A3 = D3["A"].as< array1D<double> >();

        A.set_all( L, 1 );
        A1.set(A,L1);
        A2.set(A,L2);
        A3.set(A,L3);
        
        hashing::sha1 hfn;
        size_t h = 0;
        A.hash(hfn, L1);
        h = hfn.key<size_t>();
        A1.hash(hfn,L1);
        if( hfn.key<size_t>() != h )
            throw exception("ha1 error");
        
        A.hash(hfn, L2);
        h = hfn.key<size_t>();
        A2.hash(hfn,L2);
        if( hfn.key<size_t>() != h )
            throw exception("ha2 error");
        
        A.hash(hfn, L3);
        h = hfn.key<size_t>();
        A3.hash(hfn,L3);
        if( hfn.key<size_t>() != h )
            throw exception("ha3 error");
        
    }
    
    {
        const coord2D  lo(1,1);
        const coord2D  hi(10,100);
        const layout2D L(lo,hi);
        do_split(L);
    }

    {
        const coord3D  lo(1,1,1);
        const coord3D  hi(5,10,30);
        const layout3D L(lo,hi);
        do_split(L);
    }
    
}
YOCTO_UNIT_TEST_DONE()
