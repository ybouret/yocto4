#include "yocto/utest/run.hpp"
#include "yocto/threading/team.hpp"
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
		
		void Update( team::context & ctx ) throw()
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
				const double den = i;
				local_sum += 1.0 / (den*den);
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


YOCTO_UNIT_TEST_IMPL(team)
{
	size_t ncycle = 2;
	size_t length = 100000;
	if( argc > 1 ) ncycle = atol( argv[1] );
	if( argc > 2 ) length = strtod( argv[2], NULL );
	std::cerr << ">>>> team ncycle=" << ncycle << ", length=" << length << std::endl;
	
	
	
	std::cerr << "[Testing Alone]" << std::endl;
	{
		team local_simd;
	}
	
	team simd;
	//ios::icstream in( ios::cstdin );
	//string        line;
	Compute       comp; comp.iter_per_cycles = length;
	team::task    proc( &comp, & Compute::Update );
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
				std::cerr << "sum=" << sqrt( 6.0 * comp.sum ) << " @" << i+1 << std::endr;
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
		for( size_t i=0; i < ncycle; ++i )
		{
			chrono.start();
			comp.sum = 0;
			simd.cycle( proc );
			ell += chrono.query();
			std::cerr << "sum=" << sqrt( 6.0 * comp.sum ) << " @" << i+1 << "\r";
			std::cerr.flush();
		}
		ell /= ncycle;
		std::cerr << std::endl << std::endl << "ell=" << ell * 1000.0 << " ms" << std::endl << std::endl;
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
