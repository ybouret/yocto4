#include "yocto/utest/run.hpp"
#include "yocto/math/complex.hpp"


#include "yocto/spade/array1d.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/memory/buffers.hpp"

using namespace yocto;
using namespace spade;

static inline int get_rand() { return 100 - int( 200 * alea<double>() ); }

template <typename LAYOUT>
static inline void displayLayout( const LAYOUT &L )
{
	std::cerr << "Layout" << L.dims << "D: ";
	std::cerr << L.lower << " -> " << L.upper << " : " << L.width << std::endl;
	std::cerr << "\titems=" << L.items << std::endl;
}

namespace
{
	
	
	template <typename T>
	void perform1D()
	{
		const layout1D L( get_rand(), get_rand() );
		
		array1D<T>        a(L);
		const array1D<T> &b = a;
		displayLayout( a );
		
		size_t lengths[1];
		a.request( lengths );
		
		std::cerr << "lengths[0]=" << lengths[0] << std::endl;
		memory::buffer_of<char,memory::global> wksp( lengths[0] );
		void * buffers[] = { wksp() };
		
		a.link_to( buffers );
		
		for( unit_t x=a.lower;  x <= a.upper; ++x )
		{
			a[x] = T(x);
			if( b[x] != a[x] )
				std::cerr << "mismatch" << std::endl;
		}
		
		
		
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(a1D)
{
	
	perform1D<float>();
	perform1D<double>();
	perform1D< math::complex<float> >();
	perform1D< math::complex<double> >();
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array2d.hpp"

namespace
{
	
	
	template <typename T>
	void perform2D()
	{
		const coord2D  lo( get_rand(), get_rand() );
		const coord2D  hi( get_rand(), get_rand() );
		
		const layout2D L( lo, hi );
		
		array2D<T>        a(L);
		const array2D<T> &b = a;
		displayLayout( a );
		
		size_t lengths[2];
		a.request( lengths );
		
		std::cerr << "lengths[0] (rows)=" << lengths[0] << std::endl;
		std::cerr << "lengths[1] (data)=" << lengths[1] << std::endl;
		
		size_t wlen = layout_base::len2off(  lengths, 2);
		memory::buffer_of<char,memory::global> wksp( wlen );
		assert( 0 == lengths[0] );
		assert( lengths[1] < wlen );
		void *       buffers[2] = { wksp(), wksp() + lengths[1] };
		
		a.link_to( buffers );
		
		for( unit_t x=a.lower.x;  x <= a.upper.x; ++x )
		{
			for( unit_t y=a.lower.y; y <= a.upper.y; ++y )
			{
				const coord2D p(x,y);
				a[y][x] = T(x);
				if( b[p] != a[y][x] ) std::cerr << "mismatch" << std::endl;
			}
		}
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(a2D)
{
	
	perform2D<float>();
	perform2D<double>();
	perform2D< math::complex<float> >();
	perform2D< math::complex<double> >();
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array3d.hpp"

namespace
{
	
	
	template <typename T>
	void perform3D()
	{
		const coord3D  lo( get_rand(), get_rand(), get_rand() );
		const coord3D  hi( get_rand(), get_rand(), get_rand() );
		
		const layout3D L( lo, hi );
		
		array3D<T>        a(L);
		const array3D<T> &b = a;
		displayLayout( a );
		
		size_t lengths[3];
		a.request( lengths );
		
		std::cerr << "lengths[0] (slices)=" << lengths[0] << std::endl;
		std::cerr << "lengths[1] (rows)="   << lengths[1] << std::endl;
		std::cerr << "lengths[2] (data)="   << lengths[2] << std::endl;
		
		const size_t wlen = layout_base::len2off( lengths, 3 );
		memory::buffer_of<char,memory::global> wksp( wlen );
		void *buffers[3] = { wksp(), wksp() + lengths[1], wksp() + lengths[2]  };
		
		a.link_to( buffers );
		
#if 1
		for( unit_t x=a.lower.x;  x <= a.upper.x; ++x )
		{
			for( unit_t y=a.lower.y; y <= a.upper.y; ++y )
			{
				for( unit_t z=a.lower.z; z <= a.upper.z; ++z )
				{
					const coord3D p(x,y,z);
					a[z][y][x] = T(x);
					if( b[p] != a[z][y][x] ) std::cerr << "mismatch" << std::endl;
				}
			}
		}
#endif
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(a3D)
{
	
	perform3D<float>();
	perform3D<double>();
	perform3D< math::complex<float> >();
	perform3D< math::complex<double> >();
	
}
YOCTO_UNIT_TEST_DONE()



