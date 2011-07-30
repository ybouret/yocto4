#include "yocto/utest/run.hpp"

#include "yocto/spade/workspace1d.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace spade;

template <typename LAYOUT>
static inline void displayLayout( const LAYOUT &L )
{
    std::cerr << "Layout" << L.dims << "D: ";
    std::cerr << L.lower << " -> " << L.upper << " : " << L.width << std::endl;
    std::cerr << "\titems=" << L.items << std::endl;
}


YOCTO_UNIT_TEST_IMPL(s1D)
{
	typedef workspace1D<double> wksp_t;
	hashing::sha1               H;
	
    for( size_t k=0; k < 256; ++k)
    {
        const layout1D      L( -(100+int(50*alea<double>())), (100+int(50*alea<double>())));
        const composition   C(1,2+int(5*alea<double>()) );
        wksp_t              W(C,L);
		vector<size_t>      K(W.components,0);
		
		//-- fill initial workspace
		for( unit_t x = W.lower; x <= W.upper; ++x )
		{
			for(unit_t c = W.index_min; c <= W.index_max; ++c)
			{
				W[c][x] = alea<double>() - 0.5;
			}
		}
		
		for( size_t i=0; i < W.components; ++i )
		{
			K[i+1] = H.key<size_t>( W[W.index_min+i].entry, W[ W.index_min+i ].bytes );
			std::cerr << "Key[" << i << "]=" << K[i+1] << std::endl;
		}
		
		displayLayout(W);
		
		//-- split
        for( size_t np=1; np <= 4; ++np )
        {
			std::cerr << "--- With " << np << " division(s) ---" << std::endl;
			
			for( size_t i=1; i <= W.components; ++i )
			{
				std::cerr << "\tKey=" << K[i] << std::endl;
			}
			std::cerr << "\tconstruct sub workspaces" << std::endl;
			vector< wksp_t::pointer > subW(np,as_capacity);
			
			//-- initialize
			for( size_t rank=0; rank < np; ++rank )
			{
				const layout1D  l  = W.split( np, rank, 0 );
				wksp_t::pointer pW = new wksp_t( C, l );
				subW.push_back( pW );
				std::cerr << np << ":" << rank << "."; displayLayout( *subW.back() );
				for( unit_t c=pW->index_min; c <= pW->index_max; ++c )
				{
					(*pW)[c].ld( 1.0 + rank*c );
				}
			}
			
			std::cerr << "\tsplit" << std::endl;
			//-- split
			for( size_t rank=0; rank < np; ++rank )
			{
				wksp_t &w = *subW[ rank+1 ];
				W.copy_into( w );
			}
			
			std::cerr << "\terase" << std::endl;
			//-- erase
			for( unit_t c=C.index_min; c <= C.index_max; ++c )
			{
				W[c].ldz();
				std::cerr << "\t\tKey=" << H.key<size_t>( W[c].entry, W[c].bytes ) << std::endl;
			}
			
			std::cerr << "\trefill" << std::endl;
			for( size_t rank=0; rank < np; ++rank )
			{
				wksp_t &w = *subW[ rank+1 ];
				W.fill_with( w );
			}
			
			for( unit_t c=C.index_min; c <= C.index_max; ++c )
			{
				const size_t chk = H.key<size_t>( W[c].entry, W[c].bytes );
				std::cerr << "\t\tKey=" <<  chk;
				if( chk != K[ (1+c)-C.index_min ] )
				{
					std::cerr << "\t *** ERROR ***" << std::endl;
				}
				else {
					std::cerr << "\t == OK ==" << std::endl;
					
				}
				
			}
			
			
			std::cerr << std::endl;;
        }
		
    }
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/workspace2d.hpp"

YOCTO_UNIT_TEST_IMPL(s2D)
{
	typedef workspace2D<double> wksp_t;
	hashing::sha1               H;
	
    for( size_t k=0; k < 1; ++k)
    {
		const coord2D       lo( -(100+int(50*alea<double>())), -(100+int(50*alea<double>())) );
		const coord2D       hi( (100+int(50*alea<double>())),   (100+int(50*alea<double>())) );
        const layout2D      L(lo,hi);
        const composition   C(1,2+int(5*alea<double>()) );
        wksp_t              W(C,L);
		vector<size_t>      K(W.components,0);
		
		//-- fill initial workspace
		for( unit_t x = W.lower.x; x <= W.upper.x; ++x )
		{
			for( unit_t y = W.lower.y; y <= W.upper.y; ++y )
			{
				for(unit_t c = W.index_min; c <= W.index_max; ++c)
				{
					W[c][y][x] = alea<double>() - 0.5;
				}
			}
		}
		
		for( size_t i=0; i < W.components; ++i )
		{
			K[i+1] = H.key<size_t>( W[W.index_min+i].entry, W[ W.index_min+i ].bytes );
			std::cerr << "Key[" << i << "]=" << K[i+1] << std::endl;
		}
		
		displayLayout(W);
		
		for( size_t idim=0; idim < 2; ++idim)
		{
			//-- split
			for( size_t np=1; np <= 4; ++np )
			{
				std::cerr << "--- With " << np << " division(s) in dim#" << idim << " ---" << std::endl;
				
				for( size_t i=1; i <= W.components; ++i )
				{
					std::cerr << "\tKey=" << K[i] << std::endl;
				}
				std::cerr << "\tconstruct sub workspaces" << std::endl;
				vector< wksp_t::pointer > subW(np,as_capacity);
				
				//-- initialize
				for( size_t rank=0; rank < np; ++rank )
				{
					const layout2D  l  = W.split( np, rank, idim );
					wksp_t::pointer pW = new wksp_t( C, l );
					subW.push_back( pW );
					std::cerr << np << ":" << rank << "."; displayLayout( *subW.back() );
					for( unit_t c=pW->index_min; c <= pW->index_max; ++c )
					{
						(*pW)[c].ld( 1.0 + rank*c );
					}
				}
				
				std::cerr << "\tsplit" << std::endl;
				//-- split
				for( size_t rank=0; rank < np; ++rank )
				{
					wksp_t &w = *subW[ rank+1 ];
					W.copy_into( w );
				}
				
				std::cerr << "\terase" << std::endl;
				//-- erase
				for( unit_t c=C.index_min; c <= C.index_max; ++c )
				{
					W[c].ldz();
					std::cerr << "\t\tKey=" << H.key<size_t>( W[c].entry, W[c].bytes ) << std::endl;
				}
				
				std::cerr << "\trefill" << std::endl;
				for( size_t rank=0; rank < np; ++rank )
				{
					wksp_t &w = *subW[ rank+1 ];
					W.fill_with( w );
				}
				
				for( unit_t c=C.index_min; c <= C.index_max; ++c )
				{
					const size_t chk = H.key<size_t>( W[c].entry, W[c].bytes );
					std::cerr << "\t\tKey=" <<  chk;
					if( chk != K[ (1+c)-C.index_min ] )
					{
						std::cerr << "\t *** ERROR ***" << std::endl;
					}
					else {
						std::cerr << "\t == OK ==" << std::endl;
						
					}
					
				}
				
				
				std::cerr << std::endl;;
			}
		}
    }
	
}
YOCTO_UNIT_TEST_DONE()


