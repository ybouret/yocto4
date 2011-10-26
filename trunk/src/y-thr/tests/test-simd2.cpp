#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace threading;

#include <cstdlib>

namespace {
	
	class Compute
	{
	public:
		double  sum;
		mutex   access;
		size_t  iter_per_cycles;
		
		explicit Compute() throw() : sum(0),  access( "Compute" ), iter_per_cycles(100000)
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
			{
				YOCTO_LOCK(access);
				std::cerr << "-- Update in #" << ctx.rank << "/" << ctx.size << ": @" << init << "+" << todo << std::endl;
			}
			
			double local_sum = 0;
			
			for( size_t i=init; i<last;  )
			{
				++i;
				local_sum += 1.0 / (i*i);
			}
			
			YOCTO_LOCK(access);
			sum += local_sum; 
			
		}
		
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Compute);
		
	};
	
}


YOCTO_UNIT_TEST_IMPL(SIMD)
{
	size_t np = 2;
	size_t nc = 2;
	if( argc > 1 ) np = atol( argv[1] );
	if( argc > 2 ) nc = atol( argv[2] );
	std::cerr << ">>>> SIMD " << np << " " << nc << std::endl;
	
	
	
	std::cerr << "[Testing Alone]" << std::endl;
	{
		SIMD local_simd(np);
	}
	
	SIMD simd(np);
	ios::icstream in( ios::cstdin );
	string        line;
	Compute       comp;
	SIMD::Proc    proc( &comp, & Compute::Update );
	
	while( line.clear(), fprintf(stderr,">"), fflush(stderr), in.read_line(line) >= 0 )
	{
		if( line == 'c' )
		{
			for( size_t i=0; i < nc; ++i )
			{
				//std::cerr << "[SIMD.enter cyle]" << std::endl;
				//simd.do_work = alea<double>() > 0.5;
				comp.sum = 0;
				simd.cycle( proc );
				std::cerr << "sum=" << comp.sum << std::endl;
				//std::cerr << "[SIMD.leave cyle]" << std::endl;
			}
			continue;
		}
		
		if( line == 'q' )
		{
			break;
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
