#include "yocto/utest/run.hpp"

using namespace yocto;

#include "yocto/cliff/layout.hpp"
#include "yocto/cliff/array3d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	static inline void
	gen_coord( void *p, size_t n ) throw()
	{
		unit_t *u = (unit_t *)p;
		for( size_t i=0; i < n; ++i )
		{
			u[i] = unit_t( 40 * (0.5 - alea<double>()));
		}
	}
	
	template<typename LAYOUT>
	static inline void display( const LAYOUT &L )
	{
		std::cerr << std::endl;
		std::cerr << "-- " << LAYOUT::DIMENSIONS << "D" << std::endl;
		std::cerr << "-- lower= " << L.lower << std::endl;
		std::cerr << "-- upper= " << L.upper << std::endl;
		std::cerr << "-- width= " << L.width << std::endl;
		std::cerr << "-- items= " << L.items << std::endl;
	}
	
	
	template <typename ARRAY>
	uint64_t chk( const ARRAY &A )
	{
		hashing::sha1 H;
		return H.key<uint64_t>(A.entry, A.bytes);
	}
	
	template <typename T>
	void perform1D()
	{
		coord1D U,V;
		gen_coord(&U, 1);
		gen_coord(&V, 1);
		const layout1D L(U,V);
		display(L);
		array1D<T>     A(L);
		vector<T>      data(A.items, T(0));
		A.link( &data[1] );
		A.ldz();
		
		hashing::sha1 H;
		H.set();
		for( unit_t x=A.lower; x <= A.upper; ++x )
		{
			const T v = x;
			H.run( &v, sizeof(T) );
			A[x] = v;
		}
		const uint64_t k1 = H.key<uint64_t>();
		const uint64_t k2 = chk(A);
		if( k1 != k2 )
			throw exception("array1D memory failure, level-1");
		
		H.set();
		for( unit_t x=A.lower; x <= A.upper; ++x )
		{
			H.run( &A[x], sizeof(T) );
		}
		const uint64_t k3 = H.key<uint64_t>();
		if( k1 != k3 )
			throw exception("array1D memory failure, level-2");
		
		
	}
	
	template <typename T>
	void perform2D()
	{
		coord2D U,V;
		gen_coord(&U, 2);
		gen_coord(&V, 2);
		const layout2D L(U,V);
		display(L);
		array2D<T>     A(L);
		vector<T>      data(A.items, T(0));
		A.link( &data[1] );
		A.ldz();
		
		hashing::sha1 H;
		H.set();
		for( unit_t y = A.lower.y; y <= A.upper.y; ++y )
		{
			for(unit_t x=A.lower.x; x <= A.upper.x; ++x )
			{
				const T v = x;
				H.run( &v, sizeof(T) );
				A[y][x] = v;
			}
		}
		const uint64_t k1 = H.key<uint64_t>();
		const uint64_t k2 = chk(A);
		if( k1 != k2 )
			throw exception("array2D memory failure, level-1");
		
		H.set();
		for( unit_t y = A.lower.y; y <= A.upper.y; ++y )
		{
			for(unit_t x=A.lower.x; x <= A.upper.x; ++x )
			{
				H.run( &A[y][x], sizeof(T));
			}
		}
		const uint64_t k3 = H.key<uint64_t>();
		if( k1 != k3 )
			throw exception("array2D memory failure, level-2");
		
	}
	
	template <typename T>
	void perform3D()
	{
		coord3D U,V;
		gen_coord(&U, 3);
		gen_coord(&V, 3);
		const layout3D L(U,V);
		display(L);
		array3D<T>     A(L);
		vector<T>      data(A.items, T(0));
		A.link( &data[1] );
		A.ldz();
		
		hashing::sha1 H;
		H.set();
		for( unit_t z = A.lower.z; z <= A.upper.z; ++z )
		{
			for( unit_t y = A.lower.y; y <= A.upper.y; ++y )
			{
				for(unit_t x=A.lower.x; x <= A.upper.x; ++x )
				{
					const T v = x;
					H.run( &v, sizeof(T) );
					A[z][y][x] = v;
				}
			}
		}
		const uint64_t k1 = H.key<uint64_t>();
		const uint64_t k2 = chk(A);
		if( k1 != k2 )
			throw exception("array3D memory failure, level-1");
		H.set();
		for( unit_t z = A.lower.z; z <= A.upper.z; ++z )
		{
			for( unit_t y = A.lower.y; y <= A.upper.y; ++y )
			{
				for(unit_t x=A.lower.x; x <= A.upper.x; ++x )
				{
					H.run( &A[z][y][x], sizeof(T) );
				}
				
			}
		}
		const uint64_t k3 = H.key<uint64_t>();
		if( k1 != k3 )
			throw exception("array3D memory failure, level-2");
	}		
	
	
}


YOCTO_UNIT_TEST_IMPL(array)
{
	for( size_t k=0; k < 16; ++k )
	{
		if( true )
		{
			perform1D<float>();
			perform1D<double>();
			perform1D< math::complex<float> >();
			perform1D< math::complex<double> >();
		}
		
		if( true )
		{
			perform2D<float>();
			perform2D<double>();
			perform2D< math::complex<float> >();
			perform2D< math::complex<double> >();
		}
		
		if( true )
		{
			perform3D<float>();
			perform3D<double>();
			perform3D< math::complex<float> >();
			perform3D< math::complex<double> >();
		}
	}
	
}
YOCTO_UNIT_TEST_DONE()
