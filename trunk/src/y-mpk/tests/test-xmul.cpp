
#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/random/isaac.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"


using namespace yocto;

namespace {
    
#define NPERF 2048
#define COUNT 16
    
#if defined(NDEBUG)
#	define NMAX 1024
#else
#	define NMAX 256
#endif
    
	static inline
    void test_mul( size_t N, double &tmul, double &tfft, Random::Bits &gen )
	{
		assert( N >= 2 );
        
		//-- phase 1: building the tests vector
		vector<mpn> lhs( NPERF, as_capacity );
		vector<mpn> rhs( NPERF, as_capacity );
        
        
		std::cerr << "-- initializing for " << N << " bytes (" << (N<<3) << " bits)" << std::endl;
		{
			vector<uint8_t> buffer(N,0);
			for( size_t iter=NPERF;iter>0;--iter)
			{
				const size_t nL = max_of<size_t>( gen.lt<size_t>(N), 1 );
				const size_t nR = N - nL;
				for( size_t i=1; i <= nL; ++i )
				{
					buffer[i] = gen.full<uint8_t>();
				}
                
				const mpn l( buffer(0), nL );
				lhs.push_back( l );
                
				for( size_t i=1; i <= nR; ++i )
				{
					buffer[i] = gen.full<uint8_t>();
				}
                
				const mpn r( buffer(0), nR );
				rhs.push_back( r );
                
				const mpn a = mpn::mul_( l, r );
				const mpn b = mpn::fft_( l, r );
				//std::cerr << "   " << l << "*" << r << std::endl;
				//std::cerr << " = " << a << ":" << b << std::endl;
				if( a != b )
					throw exception("fft!=mul");
			}
			assert( lhs.size() == NPERF );
			assert( rhs.size() == NPERF );
		}
        
		std::cerr << "-- benchmarking..." << std::endl;
		std::cerr.flush();
        
		wtime chrono;
		tmul = 0;
		for( size_t k=COUNT;k>0;--k)
		{
			chrono.start();
			for( size_t i=NPERF;i>0;--i)
				(void) mpn::mul_( lhs[i], rhs[i] );
			tmul += chrono.query();
			(std::cerr << '.' ).flush();
		}
        
		tfft = 0;
		for( size_t k=COUNT;k>0;--k)
		{
			chrono.start();
			for( size_t i=NPERF;i>0;--i)
				(void) mpn::fft_( lhs[i], rhs[i] );
			tfft += chrono.query();
			(std::cerr << '#' ).flush();
		}
		std::cerr << std::endl;
		const size_t nops = COUNT * NPERF;
		tmul *= 1.0e6;
		tfft *= 1.0e6;
		tmul /= nops;
		tfft /= nops;
        
	}
    
}

#include <cstdio>

YOCTO_UNIT_TEST_IMPL(xmul)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	double tmul=0,tfft=0;
    
	{
		ios::ocstream fp("xmul.dat",false);
	}
    
    
	for( size_t N=2; N <= NMAX; N <<= 1 )
	{
		test_mul(N,tmul,tfft,gen);
		std::cerr.flush();
		fprintf(stderr, "\tTimings: <mul>= %10.5f | <fft>= %10.5f\n\n", tmul, tfft);
		fflush(stderr);
		ios::ocstream fp("xmul.dat",true);
		fp("%u %.6g %.6g\n", (unsigned)N, tmul, tfft );
	}
    
}
YOCTO_UNIT_TEST_DONE()


static inline
void test_sqr( size_t N, double &tmul, double &tfft, double &tsqr, Random::Bits &gen )
{
	assert( N >= 2 );
    
	//-- phase 1: building the test vector
	vector<mpn> lhs( NPERF, as_capacity );
    
	std::cerr << "-- initializing for " << N << " bytes (" << (N<<3) << " bits)" << std::endl;
	{
		vector<uint8_t> buffer(N,0);
		for( size_t iter=NPERF;iter>0;--iter)
		{
			for( size_t i=1; i <= N; ++i )
			{
				buffer[i] = gen.full<uint8_t>();
			}
			const mpn l( buffer(0), N );
			lhs.push_back( l );
		}
        
	}
    
	std::cerr << "-- checking..." << std::endl;
	std::cerr.flush();
    
	for( size_t i=NPERF;i>0;--i)
	{
		const mpn & l = lhs[i];
		const mpn a = mpn::mul_( l,l );
		const mpn b = mpn::fft_( l,l );
		const mpn c = mpn::sqr_( l   );
		if( a != b || a != c || b != c )
		{
			throw exception("sqr mismatch");
		}
	}
    
	std::cerr << "-- benchmarking..." << std::endl;
	std::cerr.flush();
    
	wtime chrono;
	tmul = 0;
	for( size_t k=COUNT;k>0;--k)
	{
		chrono.start();
		for( size_t i=NPERF;i>0;--i)
			(void) mpn::mul_( lhs[i], lhs[i] );
		tmul += chrono.query();
		(std::cerr << '.' ).flush();
	}
    
    
	tfft = 0;
	for( size_t k=COUNT;k>0;--k)
	{
		chrono.start();
		for( size_t i=NPERF;i>0;--i)
			(void) mpn::fft_( lhs[i], lhs[i] );
		tfft += chrono.query();
		(std::cerr << '#' ).flush();
	}
    
	tsqr = 0;
	for( size_t k=COUNT;k>0;--k)
	{
		chrono.start();
		for( size_t i=NPERF;i>0;--i)
			(void) mpn::sqr_( lhs[i] );
		tsqr += chrono.query();
		(std::cerr << '*' ).flush();
	}
	(std::cerr << std::endl).flush();
    
    
	const size_t nops = COUNT * NPERF;
	tmul *= 1.0e6;
	tfft *= 1.0e6;
	tsqr *= 1.0e6;
	tmul /= nops;
	tfft /= nops;
	tsqr /= nops;
    
}

YOCTO_UNIT_TEST_IMPL(xsqr)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	double tmul=0,tfft=0,tsqr=0;
    
	{
		ios::ocstream fp("xsqr.dat",false);
	}
    
	for( size_t N=2; N <= NMAX; N <<= 1 )
	{
		test_sqr(N,tmul,tfft,tsqr,gen);
		ios::ocstream fp("xsqr.dat",true);
		fp("%u %.6g %.6g %.6g\n", (unsigned)N, tmul, tfft, tsqr);
	}
    
}
YOCTO_UNIT_TEST_DONE()
