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
using namespace math;

namespace 
{
	
	const unit_t umin = -20;
	const unit_t umax =  20;
	const unit_t ulen = umax - umin;
#define INSIDE (umin/2+unit_t( ulen * ( alea<double>() * 0.5 ) ) )
	
	template <typename T>
	static inline void perform1D( )
	{
		std::cerr << "Testing 1D" << std::endl;
		hashing::sha1 H;
		
		layout1D   L(umin,umax);
		array1D<T> A(L);
		array1D<T> B(L);
		vector<T>  Adat(L.items,T(0));
		vector<T>  Bdat(L.items,T(0));
		
		A.link( &Adat[1] );
		B.link( &Bdat[1] );
		
		//-- fill A
		const double xfac = 1.0 / (A.upper-A.lower);
		for( unit_t x=A.lower; x <= A.upper; ++x )
		{
			const double X = (x-A.lower) * xfac;
			const double F = sin( X * numeric<double>::pi/2 );
			A[x] = F;
		}
		const uint64_t A_key = H.key<uint64_t>( A.entry, A.bytes );
		
		//-- fill B piecewise
		for( size_t size=1; size <= 8; ++size )
		{
			B.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout1D Lr = L.split(rank,size);
				B.load(A,Lr);
			}
			const uint64_t B_key = H.key<uint64_t>( B.entry, B.bytes );
			if( A_key != B_key )
				throw exception("size=%u: Error level 1", unsigned(size));
			A.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout1D Lr = L.split(rank,size);
				B.save(A,Lr);
			}
			const uint64_t C_key = H.key<uint64_t>( A.entry, A.bytes );
			if( A_key != C_key )
				throw exception("size=%u: Error level 2", unsigned(size));
		}
		
		//-- random chunks...
		for(size_t k=0; k < 16; ++k )
		{
			B.ldz();
			const layout1D L1( INSIDE, INSIDE );
			B.load(A,L1);
			B.save(A,L1);
			if( H.key<uint64_t>( A.entry, A.bytes ) != A_key )
				throw exception("random access error");
		}
	}
	
	template <typename T>
	static inline void perform2D( )
	{
		std::cerr << "Testing 2D" << std::endl;
		hashing::sha1 H;
		
		layout2D   L( coord2D(umin,umin), coord2D(umax,umax) );
		array2D<T> A(L);
		array2D<T> B(L);
		vector<T>  Adat(L.items,T(0));
		vector<T>  Bdat(L.items,T(0));
		
		A.link( &Adat[1] );
		B.link( &Bdat[1] );
		
		//-- fill A
		const double xfac = 1.0 / (A.upper.x-A.lower.x);
		const double yfac = 1.0 / (A.upper.y-A.lower.y);
		for(unit_t y=A.lower.y; y <= A.upper.y; ++y )
		{
			const double Y = (y-A.lower.y) * yfac;
			for( unit_t x=A.lower.x; x <= A.upper.x; ++x )
			{
				const double X = (x-A.lower.x) * xfac;
				const double F = sin( (X+Y) * numeric<double>::pi/2 );
				A[y][x] = F;
			}
		}
		const uint64_t A_key = H.key<uint64_t>( A.entry, A.bytes );
		
		
		
		
		//-- fill B piecewise
		for( size_t size=1; size <= 8; ++size )
		{
			B.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout2D Lr = L.split(rank,size);
				B.load(A,Lr);
			}
			const uint64_t B_key = H.key<uint64_t>( B.entry, B.bytes );
			if( A_key != B_key )
				throw exception("size=%u: Error level 1", unsigned(size));
			A.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout2D Lr = L.split(rank,size);
				B.save(A,Lr);
			}
			const uint64_t C_key = H.key<uint64_t>( A.entry, A.bytes );
			if( A_key != C_key )
				throw exception("size=%u: Error level 2", unsigned(size));
		}
		
		//-- random chunks...
		for(size_t k=0; k < 16; ++k )
		{
			B.ldz();
			const layout2D L1( coord2D(INSIDE, INSIDE), coord2D(INSIDE,INSIDE) );
			B.load(A,L1);
			B.save(A,L1);
			if( H.key<uint64_t>( A.entry, A.bytes ) != A_key )
				throw exception("random access error");
		}
		
		
	}
	
	
	template <typename T>
	static inline void perform3D( )
	{
		std::cerr << "Testing 3D" << std::endl;
		hashing::sha1 H;
		
		layout3D   L( coord3D(umin,umin,umin), coord3D(umax,umax,umax) );
		array3D<T> A(L);
		array3D<T> B(L);
		vector<T>  Adat(L.items,T(0));
		vector<T>  Bdat(L.items,T(0));
		
		A.link( &Adat[1] );
		B.link( &Bdat[1] );
		
		//-- fill A
		const double xfac = 1.0 / (A.upper.x-A.lower.x);
		const double yfac = 1.0 / (A.upper.y-A.lower.y);
		const double zfac = 1.0 / (A.upper.z-A.lower.z);
		
		for( unit_t z=A.lower.z; z <= A.upper.z; ++z)
		{
			const double Z = (z-A.lower.z) * zfac;
			for(unit_t y=A.lower.y; y <= A.upper.y; ++y )
			{
				const double Y = (y-A.lower.y) * yfac;
				for( unit_t x=A.lower.x; x <= A.upper.x; ++x )
				{
					const double X = (x-A.lower.x) * xfac;
					const double F = sin( (X+Y+Z) * numeric<double>::pi/2 );
					A[z][y][x] = F;
				}
			}
		}
		const uint64_t A_key = H.key<uint64_t>( A.entry, A.bytes );
		
				
		//-- fill B piecewise
		for( size_t size=1; size <= 8; ++size )
		{
			B.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout3D Lr = L.split(rank,size);
				B.load(A,Lr);
			}
			const uint64_t B_key = H.key<uint64_t>( B.entry, B.bytes );
			if( A_key != B_key )
				throw exception("size=%u: Error level 1", unsigned(size));
			A.ldz();
			for( size_t rank=0; rank < size; ++rank )
			{
				const layout3D Lr = L.split(rank,size);
				B.save(A,Lr);
			}
			const uint64_t C_key = H.key<uint64_t>( A.entry, A.bytes );
			if( A_key != C_key )
				throw exception("size=%u: Error level 2", unsigned(size));
		}
		
		//-- random chunks...
		for(size_t k=0; k < 16; ++k )
		{
			B.ldz();
			const layout3D L1( coord3D(INSIDE, INSIDE, INSIDE), coord3D(INSIDE,INSIDE,INSIDE) );
			B.load(A,L1);
			B.save(A,L1);
			if( H.key<uint64_t>( A.entry, A.bytes ) != A_key )
				throw exception("random access error");
		}
		
		
	}
	
	
}


YOCTO_UNIT_TEST_IMPL(split)
{
	{
		perform1D<float>();
		perform1D<double>();
		perform1D< complex<float> >();
		perform1D< complex<double> >();
	}
	
	{
		perform2D<float>();
		perform2D<double>();
		perform2D< complex<float> >();
		perform2D< complex<double> >();
	}
	
	{
		perform3D<float>();
		perform3D<double>();
		perform3D< complex<float> >();
		perform3D< complex<double> >();
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
