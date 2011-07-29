#include "yocto/utest/run.hpp"
#include "yocto/random/uniform-isaac.hpp"
#include "yocto/random/uniform0.hpp"
#include "yocto/random/uniform1.hpp"
#include "yocto/random/uniform2.hpp"
#include "yocto/random/uniform3.hpp"
#include "yocto/random/uniform-mt.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/code/hsort.hpp"
#include <cmath>

using namespace yocto;


static inline void test_random( Random::Uniform &ran )
{
	const size_t   n = 10000;
	vector<double> r(n,0);
	for( size_t i=1; i <=n; ++i ) r[i] = ran();
	hsort( r );
	double ave = 0;
	for( size_t i=1; i <=n; ++i ) ave += r[i];
	ave /= n;
	std::cerr << "-- ave= " << ave << std::endl;
	double sig = 0;
	for( size_t i=1; i <=n; ++i )
	{
		const double dr = r[i] - ave;
		sig += dr*dr;
	}
	sig = sqrt( sig/(n-1) );
	std::cerr << "-- sig= " << sig << std::endl;
	
	std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(dist)
{
	Random::isaac_fast   ran_if( Random::SimpleBits() );
	Random::isaac_crypto ran_ic( Random::SimpleBits() );
	Random::Uniform0     ran0(   Random::SimpleBits() );
	Random::Uniform1     ran1(   Random::SimpleBits() );
	Random::Uniform2     ran2(   Random::SimpleBits() );
	Random::Uniform3     ran3(   Random::SimpleBits() );
	Random::UniformMT    ranMT(  Random::SimpleBits() );
	
	Random:: Uniform * reg[] = { &ran_if, & ran_ic, &ran0, &ran1, &ran2, &ran3, &ranMT };
	const size_t       num   = sizeof(reg)/sizeof(reg[0]);
	
	for( size_t i=0; i < num; ++i )
	{
		test_random( *reg[i] );
	}
	
}
YOCTO_UNIT_TEST_DONE()

