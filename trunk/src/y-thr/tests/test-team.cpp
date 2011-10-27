#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/wtime.hpp"

using namespace yocto;
using namespace threading;

#include <cstdlib>
#include <cmath>

namespace {
	
	class Compute
	{
	public:
		double  sum;
		size_t  iter_per_cycles;
		
		explicit Compute() throw() : sum(0), iter_per_cycles(100000)
		{
		}
		
		virtual ~Compute() throw()
		{
			
		}
		
		void Update( SIMD::Args ctx ) throw()
		{
			
			const size_t todo = iter_per_cycles / ctx.size;
			const size_t init = todo * ctx.rank;
			const size_t last = init + todo;
			
			/*
			 {
			 YOCTO_LOCK(ctx.access);
			 std::cerr << "-- Update in #" << ctx.rank << "/" << ctx.size << ": @" << init << "+" << todo << std::endl;
			 }
			 */
			
			double local_sum = 0;
			
			for( size_t i=init; i<last;  )
			{
				++i;
				local_sum += 1.0 / (i*i);
			}
			
			{
				YOCTO_LOCK(ctx.access);
				sum += local_sum; 
			}
		}
		
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Compute);
		
	};
	
}


YOCTO_UNIT_TEST_IMPL(SIMD)
{
	size_t np = 2;
	size_t nc = 2;
	size_t nl = 100000;
	if( argc > 1 ) np = atol( argv[1] );
	if( argc > 2 ) nc = atol( argv[2] );
	if( argc > 3 ) nl = size_t( strtod(argv[3], NULL) );
	std::cerr << ">>>> SIMD " << np << " " << nc << " " << nl << std::endl;
	
	
	
	std::cerr << "[Testing Alone]" << std::endl;
	{
		SIMD local_simd(np);
	}
	
	SIMD simd(np);
	//ios::icstream in( ios::cstdin );
	//string        line;
	Compute       comp; comp.iter_per_cycles = nl;
	SIMD::Proc    proc( &comp, & Compute::Update );
	wtime         chrono;
	
#if 0
	while( line.clear(), fprintf(stderr,">"), fflush(stderr), in.read_line(line) >= 0 )
	{
		if( line == 'c' )
		{
			double ell = 0;
			for( size_t i=0; i < nc; ++i )
			{
				chrono.start();
				comp.sum = 0;
				simd.cycle( proc );
				ell += chrono.query();
				std::cerr << "sum=" << sqrt( 6.0 * comp.sum ) << std::endl;
			}
			ell /= nc;
			std::cerr << "ell=" << ell * 1000.0 << " ms" << std::endl;
			continue;
		}
		
		if( line == 'q' )
		{
			break;
		}
	}
#endif
	
	{
		double ell = 0;
		for( size_t i=0; i < nc; ++i )
		{
			chrono.start();
			comp.sum = 0;
			simd.cycle( proc );
			ell += chrono.query();
			std::cerr << "sum=" << sqrt( 6.0 * comp.sum ) << std::endl;
		}
		ell /= nc;
		std::cerr << std::endl << "ell=" << ell * 1000.0 << " ms" << std::endl << std::endl;
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
