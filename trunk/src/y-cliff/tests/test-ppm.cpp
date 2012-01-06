#include "yocto/utest/run.hpp"


#include "yocto/cliff/array2d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace cliff;


static inline double vproc( const float &v )
{
	return v;
}
YOCTO_UNIT_TEST_IMPL(ppm)
{
	const unit_t   N = 100;
	layout2D       L( coord2D(0,0), coord2D(N,N) );
	array2D<float> A(L);
	vector<float>  data(A.items,0);
	A.link( &data[1] );

	A.ldz();
	const double xfac = 1.0 / N;
	const double yfac = 1.0 / N;
	for( unit_t y=A.lower.y; y <= A.upper.y; ++y )
	{
		const double Y = yfac * y;
		for( unit_t x=A.lower.x; x <= A.upper.x; ++x)
		{
			const double X = xfac * x;
			A[y][x] = sin( (X+Y) * 4.0 );
		}
	}
	float amin=0,amax=0;
	A.get_min_max(amin,NULL,amax,NULL);
	std::cerr << "amin= " << amin << std::endl;
	std::cerr << "amax= " << amax << std::endl;

	A.ppm("a.ppm", "full", A, vproc, NULL, amin, amax);
	
	for( size_t size=1; size<=3; ++size)
	{
		for(size_t rank=0;rank<size;++rank)
		{
			const layout2D Lr( L.split(rank,size) );
			array2D<float> B( Lr );
			vector<float>  tmp( B.items, 0 );
			B.link( &tmp[1] );
			B.set(A,Lr);
			B.ppm( vformat("a%u%u.ppm", unsigned(size),unsigned(rank)), "full", B,  vproc, NULL, amin, amax);
		}
	}
	
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
