#include "yocto/utest/run.hpp"

#include "yocto/random/default.hpp"
#include "yocto/random/uniform-isaac.hpp"
#include "yocto/random/uniform0.hpp"
#include "yocto/random/uniform1.hpp"
#include "yocto/random/uniform2.hpp"
#include "yocto/random/uniform3.hpp"
#include "yocto/random/uniform-mt.hpp"
#include "yocto/random/uniform64.hpp"
#include "yocto/random/gaussian.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sort/heap.hpp"
#include <cmath>

using namespace yocto;


static inline void test_random( Random::Uniform &ran )
{
	const size_t   n = 100000;
	vector<double> r(n,0);
	for( size_t i=1; i <=n; ++i ) r[i] = ran();
	hsort( r );
	double ave = 0;
	for( size_t i=1; i <=n; ++i ) ave += r[i];
	ave /= n;
	double sig = 0;
	for( size_t i=1; i <=n; ++i )
	{
		const double dr = r[i] - ave;
		sig += dr*dr;
	}
	sig = sqrt( sig/(n-1) );
	std::cerr << "-- normal:   ave= " << ave << "| sig= " << sig << std::endl;
	
	Random::Gaussian gran( ran );
    double sq = 0;
	for( size_t i=1; i <=n; ++i ) r[i] = gran();
	hsort( r );
	ave = 0;
	for( size_t i=1; i <=n; ++i )
    {
        ave += r[i];
        sq  += r[i] * r[i];
    }
	ave /= n;
    sq  /= n;
	sig = 0;
	for( size_t i=1; i <=n; ++i )
	{
		const double dr = r[i] - ave;
		sig += dr*dr;
	}
	sig = sqrt( sig/(n-1) );
	std::cerr << "-- gaussian: ave= " << ave << "| sig= " << sig << " | <sq>=" << sq << std::endl;
	
	std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(dist)
{
	Random::Default      ran_d(  Random::SimpleBits() );
	Random::isaac_fast   ran_if( Random::SimpleBits() );
	Random::isaac_crypto ran_ic( Random::SimpleBits() );
	Random::Uniform0     ran0(   Random::SimpleBits() );
	Random::Uniform1     ran1(   Random::SimpleBits() );
	Random::Uniform2     ran2(   Random::SimpleBits() );
	Random::Uniform3     ran3(   Random::SimpleBits() );
	Random::UniformMT    ranMT(  Random::SimpleBits() );
	Random::Uniform64BJ  ranBJ(  Random::SimpleBits() );
	Random::Uniform64NR  ranNR(  Random::SimpleBits() );

	Random:: Uniform * reg[] = { &ran_d, &ran_if, & ran_ic, &ran0, &ran1, &ran2, &ran3, &ranMT, &ranBJ, &ranNR };
	const size_t       num   = sizeof(reg)/sizeof(reg[0]);
	
	for( size_t i=0; i < num; ++i )
	{
		test_random( *reg[i] );
	}
	
}
YOCTO_UNIT_TEST_DONE()

