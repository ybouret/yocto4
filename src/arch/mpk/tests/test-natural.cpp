#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/random/isaac.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/bswap.hpp"

using namespace yocto;


namespace
{
	struct blk_t
	{
		size_t   n;
		uint8_t *p;
	};
}

#include <cstdlib>

#define N 16384
#define M 10


YOCTO_UNIT_TEST_IMPL(mem)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );

	blk_t blk[ N ];
	
	wtime   chrono;
	double  ell = 0;
	
	for( size_t k=0; k < M; ++k)
	{
		chrono.start();
		for( size_t i=0; i < N; ++i )
		{
			blk_t &b = blk[i];
			b.n      = 1+gen.lt<size_t>( 1024 );
			b.p      = mpk::mem_acquire( b.n );
		}
		ell += chrono.query();
		c_shuffle( blk, N );
		
		chrono.start();
		for( size_t i=0; i < N; ++i )
		{
			blk_t &b = blk[i];
			mpk::mem_release( b.p, b.n );
		}
		ell += chrono.query();
	}
	std::cerr << "ell=" << ell << std::endl;
	std::cerr << "spd=" << double(N*M) / ell << std::endl;
}
YOCTO_UNIT_TEST_DONE()

#define ITER 1024
//typedef mpk::natural mpn;

template <typename T>
static void proc_disp( Random::Bits &gen )
{
	for( size_t i=0; i < ITER; ++i )
	{
		{
			T       u = gen.fuzz<T>();
			mpn     X = u;
			mpn     Y( X );
			mpn     Z;
			Z = X;
			
			std::cerr << std::hex << "u=" << int(u) << ", X=" << X << ", Y=" << Y << ", Z=" << Z << " | bits=" << std::dec << X.bits() << " | => " << std::hex << uint32_t(X.to<T>()) << std::endl;
		}
	}
}

YOCTO_UNIT_TEST_IMPL(natural)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	std::cerr << "-- Testing Get/Set" << std::endl;
	proc_disp<uint8_t>( gen );
	proc_disp<uint16_t>( gen );
	proc_disp<uint32_t>( gen );
	proc_disp<uint64_t>( gen );
	
    
	{
		std::cerr << "-- Testing Addition" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t lhs32 = gen.full<uint64_t>()/2;
			const uint32_t rhs32 = gen.fuzz<uint32_t>()/2;
			const mpn      lhs   = lhs32;
			const mpn      rhs   = rhs32;
			const mpn      sum1  = mpn::add_( lhs, rhs );
			const mpn      sum2  = mpn::add_( rhs, lhs );
			const uint64_t s1    = sum1.to<uint64_t>();
			const uint64_t s2    = sum2.to<uint64_t>();
			const uint64_t res   = uint64_t(lhs32) + uint64_t(rhs32);
			std::cerr << std::hex << lhs32 << " + " << rhs32 << " = " << res << " => ";
			std::cerr << lhs << " + " << rhs << " = " << s1 << ", " << s2 << std::endl;
			if( res != s1 || res != s2 )
				throw exception("ADD failure");
		}
	}
	
	{
		std::cerr << "-- Testing Comparison" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t u = gen.fuzz<uint64_t>();
			const uint64_t v = gen.fuzz<uint64_t>();
			std::cerr << std::hex << u << ", " << v;
			const mpn lhs = u;
			const mpn rhs = v;
			std::cerr << "  =>  " << lhs << ", " << rhs;
			std::cerr << std::dec << "(" << mpn::compare(lhs,rhs)
			<< "," << mpn::compare(lhs,lhs)
			<< "," << mpn::compare(rhs,rhs)
			<< "," << mpn::compare(rhs,lhs)
			<< ")";
			std::cerr << std::endl;
		}
		
	}
	
	{
		std::cerr << "-- Testing Subtraction" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			uint64_t u = gen.fuzz<uint64_t>();
			uint64_t v = gen.fuzz<uint64_t>();
			if( u < v ) cswap(u,v);
			
			std::cerr << std::hex << u << "-" << v << "=" << (u-v);
			const mpn lhs = u;
			const mpn rhs = v;
			const mpn res = mpn::sub_(lhs,rhs);
			std::cerr << "  =>  " << lhs << "-" << rhs << "=" << res;
			std::cerr << std::endl;
			
			if( u-v != res.to<uint64_t>() )
				throw exception("SUB failure");
		}
	}
	
	{
		std::cerr << "-- Testing SHL" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			uint64_t      u   = gen.fuzz<uint64_t>();
			const mpn    lhs = u;
			const size_t n = 1 + gen.lt<size_t>(10);
			const mpn    res = mpn::shl_( lhs, n );
			std::cerr << "0x" << lhs << " << " << std::dec << n << " = 0x" << res << std::endl;
			if( res.bits() <= 64 )
			{
				const uint64_t r64 = res.to<uint64_t>();
				std::cerr << std::dec << "\t " << u << " << " << n << " = " << r64 << std::endl;
				if( (u<<n) != r64 )
					throw exception("SHL Failure");
			}
		}
		
	}
	
	{
		std::cerr << "-- Testing SHR" << std::endl;
		
		for( size_t k=0; k< ITER; ++k )
		{
			uint64_t      u   = gen.full<uint64_t>();
			const mpn     lhs = u;
			const size_t  n   = gen.lt<size_t>( 64 );
			const mpn     res = mpn::shr_( lhs, n );
			std::cerr << "0x" << lhs << " >> " << std::dec << n << " = 0x" << res << std::endl;
			
			const uint64_t r64 = res.to<uint64_t>();
			std::cerr << std::hex << "\t 0x" << u << " >> " << n << " = 0x" << r64 << std::endl;
			if( (u >> n) != r64 ) throw exception("SHR failure level 1");
		}
		
		for( size_t k=0; k< ITER; ++k )
		{
			uint64_t      u   = gen.fuzz<uint64_t>();
			const mpn     lhs = u;
			mpn           res = lhs;
			res.shr();
			std::cerr << lhs << ".shr = " << res << std::endl;
			
			const uint64_t r64 = res.to<uint64_t>();
			std::cerr << std::dec << "\t " << u << ".shr = " << r64 << std::endl;
			if( (u >> 1) != r64 ) throw exception("SHR failure level 2");
		}
		
	}
	
	//return 0;
	
	{
		std::cerr << "-- Testing MUL" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t l32 = gen.fuzz<uint32_t>();
			const uint64_t r32 = gen.full<uint32_t>();
			const mpn      lhs = l32;
			const mpn      rhs = r32;
			const mpn      m1  = mpn::mul_( lhs, rhs );
			const mpn      m2  = mpn::mul_( rhs, lhs );
			if( m1 != m2 )
			{
				std::cerr << "lhs=" << lhs << std::endl;
				std::cerr << "rhs=" << rhs << std::endl;
				std::cerr << "m1 =" << m1 << std::endl;
				std::cerr << "m2 =" << m2 << std::endl;
				throw exception("MUL failure level-1");
				
			}
			const uint64_t r0  = l32 * r32;
			const uint64_t r1  = m1.to<uint64_t>();
			const uint64_t r2  = m2.to<uint64_t>();
			std::cerr << lhs << " * " << rhs << " = " << m1 << ", " << m2 << " / " << std::hex << r0 << " <" << r1 << "," << r2 << ">" << std::endl;
			if( r1 != r0 || r2 != r0 ) throw exception("MUL failure level 2");
		}
		
	}
	
	
	
	{
		std::cerr << "-- Testing FFT" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t l32 = gen.fuzz<uint32_t>();
			const uint64_t r32 = gen.full<uint32_t>();
			//const uint64_t r32 = gen.partial<uint32_t>();
			
			const mpn      lhs = l32;
			const mpn      rhs = r32;
			const uint64_t r0  = l32 * r32;
			const mpn      m1  = mpn::fft_( lhs, rhs );
			
			
			const mpn      m2  = mpn::fft_( rhs, lhs );
			std::cerr << lhs << " * " << rhs << " => m1=" << m1 << ", "
			<< "m2=" << m2
			<< " / " << std::hex << r0
			<< std::endl;
			
			if( m1 != m2 )
			{
				throw exception("FFT failure level-1");
				
			}
			
			const uint64_t r1  = m1.to<uint64_t>();
			const uint64_t r2  = m2.to<uint64_t>();
			if( r1 != r0 || r2 != r0 ) throw exception("FFT failure level 2");
		}
		
	}
	
	{
		std::cerr << "-- Testing EXP2" << std::endl;
		for( size_t k=0; k<= 70; ++k )
		{
			const mpn ans = mpn::exp2(k);
			std::cerr << "1 << " << std::dec << k << " = " << ans << " (bits=" << std::dec << ans.bits() << ")" << std::endl; 
			
		}
		
	}
	
	{
		std::cerr << "-- Testing DIV" << std::endl;
		for( size_t k=0; k < ITER; ++k )
		{
			const uint64_t n32 = gen.full<uint64_t>();
			const uint64_t d32 = 1 + gen.fuzz<uint64_t>();
			const uint64_t q32 = n32/d32;
			const mpn num = n32;
			const mpn den = d32;
			std::cerr << std::hex << "0x" << n32 << " / 0x" << d32 << " = 0x" << std::hex << q32 << std::endl;
			const mpn q   = mpn::div_(num,den);
			std::cerr << "=> q= 0x" << q << std::endl;
			if( q32 != q.to<uint64_t>() )
				throw exception("DIV failure");
		}
		
	}
	
	{
		std::cerr << "-- Testing MOD" << std::endl;
		for( size_t k=0; k < ITER; ++k )
		{
			const uint64_t n32 = gen.full<uint64_t>();
			const uint64_t d32 = 1 + gen.fuzz<uint64_t>();
			const uint64_t q32 = n32 % d32;
			const mpn num = n32;
			const mpn den = d32;
			std::cerr << std::hex << "0x" << n32 << " % 0x" << d32 << " = 0x" << std::hex << q32 << std::endl;
			const mpn q   = mpn::mod_(num,den);
			std::cerr << "=> r = 0x" << q << std::endl;
			if( q32 != q.to<uint64_t>() )
				throw exception("MOD failure");
		}
		
	}
	
	{
		std::cerr << "-- Testing MOD_INV" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t      n32   = 1+gen.full<uint32_t>();
			const uint64_t      b32   = gen.full<uint64_t>();
			const mpn           n     = n32;
			const mpn           b     = b32;
			const mpn           m     = mpn::mod_inv( b, n );
			std::cerr << b << "^(-1)[" << n << "]=" << m << std::endl;
		}
	}
	
	{
		std::cerr << "-- Testing MOD_EXP" << std::endl;
		for( size_t k=0; k< ITER; ++k )
		{
			const uint64_t      n32   = 1+gen.full<uint32_t>();
			const uint64_t      b32   = gen.full<uint64_t>();
			const uint64_t      e32   = gen.full<uint16_t>();
			const mpn           n     = n32;
			const mpn           b     = b32;
			const mpn           e     = e32;
			const mpn           m     = mpn::mod_exp( b, e, n );
			std::cerr << b << "^" << e << "[" << n << "]=" << m << std::endl;
		}
	}
	
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/vector.hpp"
#define SZMAX 512
#define COUNT 128

static inline void genvec( vector<mpn> &arr, size_t NN )
{
	arr.free();
	arr.reserve(NN);
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	for( size_t i=NN;i>0;--i)
	{
		uint8_t      buffer[SZMAX];
		const size_t buflen = 1+gen.lt<size_t>(SZMAX); assert( buflen <= SZMAX );
		for( size_t j=0; j < buflen; ++j ) buffer[j] = gen.full<uint8_t>();
		const mpn    tmp( buffer, buflen );
		//std::cerr << "mpn=" << tmp << std::endl;
		arr.push_back( tmp );
	}
	
}

YOCTO_UNIT_TEST_IMPL(addn_perf)
{
	const size_t NPERF = 16384;
	vector<mpn> arr;
	genvec(arr,NPERF);
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		chrono.start();
		
		for( size_t i=1; i < NPERF; ++i )
		{
			const mpn &lhs = arr[i];
			const mpn &rhs = arr[i+1];
			(void) mpn::add_( lhs,rhs);
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
		
	}
	std::cerr << std::endl;
	const double spd = ((COUNT*(NPERF-1)) / ell) * 1.0e-6;
	std::cerr << "add speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(cmpn_perf)
{
	const size_t NPERF = 16384;
	vector<mpn> arr;
	genvec(arr,NPERF);
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		chrono.start();
		
		for( size_t i=1; i < NPERF; ++i )
		{
			const mpn &lhs = arr[i];
			const mpn &rhs = arr[i+1];
			(void) mpn::compare( lhs, rhs );
			(void) mpn::compare( lhs, lhs );
			(void) mpn::compare( rhs, rhs );
			(void) mpn::compare( rhs, lhs );
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
	}
	std::cerr << std::endl;
	const double spd = 4.0 * ((COUNT*(NPERF-1)) / ell) * 1.0e-6;
	std::cerr << "cmp speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(muln_perf)
{
	const size_t MPERF = 512;
	vector<mpn> arr;
	genvec(arr,MPERF);
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		chrono.start();
		
		for( size_t i=1; i < MPERF; ++i )
		{
			const mpn &lhs = arr[i];
			const mpn &rhs = arr[i+1];
			(void) mpn::mul_( lhs,rhs);
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
	}
	std::cerr << std::endl;
	const double spd = ((COUNT*(MPERF-1)) / ell) * 1.0e-6;
	std::cerr << "mul speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(fftn_perf)
{
	const size_t MPERF = 512;
	vector<mpn> arr;
	genvec(arr,MPERF);
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		chrono.start();
		
		for( size_t i=1; i < MPERF; ++i )
		{
			const mpn &lhs = arr[i];
			const mpn &rhs = arr[i+1];
			(void) mpn::fft_( lhs,rhs);
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
	}
	std::cerr << std::endl;
	const double spd = ((COUNT*(MPERF-1)) / ell) * 1.0e-6;
	std::cerr << "fft speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(divn_perf)
{
	const size_t MPERF = 2;
	vector<mpn> arr;
	genvec(arr,MPERF);
	for( size_t i=1; i <= MPERF; ++i ) arr[i].inc(1);
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		chrono.start();
		
		for( size_t i=1; i < MPERF; ++i )
		{
			const mpn &lhs = arr[i];
			const mpn &rhs = arr[i+1];
			(void) mpn::div_(lhs,rhs);
			(void) mpn::div_(rhs,lhs);
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
	}
	std::cerr << std::endl;
	const double spd = 2.0*((COUNT*(MPERF-1)) / ell) * 1.0e-6;
	std::cerr << "div speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(shrn_perf)
{
	const size_t MPERF = 512;
	vector<mpn> arr;
	
	
	wtime chrono;
	double ell = 0;
	for( size_t k=0; k < COUNT; ++k)
	{
		genvec(arr,MPERF);
		chrono.start();
		
		for( size_t i=1; i < MPERF; ++i )
		{
			arr[i].shr();
		}
		ell += chrono.query();
		(std::cerr << '.').flush();
	}
	std::cerr << std::endl;
	const double spd = ((COUNT*(MPERF-1)) / ell) * 1.0e-6;
	std::cerr << "shr speed= " << spd << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()






