#include "yocto/utest/run.hpp"
#include "yocto/threading/crew.hpp"
#include "yocto/wtime.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>
#include <cmath>

using namespace yocto;
using namespace memory;
namespace 
{
	static size_t  num = 0;
	
	struct slice_t
	{
		size_t offset;
		size_t length;
		mutable double result;
	};
	
	static 
	void func( threading::crew::context &ctx, void *args ) throw()
	{
		const slice_t &slice = *(slice_t *)args;
		if( true )
		{
			scoped_lock _lock( ctx.critical );
			++num;
			std::cerr << "Performing function, rank=" << ctx.rank << " num=" << num 
			<< " @" << slice.offset << "+1.." << slice.length << std::endl;
		}
		double ans = 0;
		size_t j=slice.offset+slice.length;
		for( size_t i=slice.length;i>0;--i,--j)
		{
			ans += 1.0 / (double(j)*double(j));
		}
		
		slice.result = ans;
		
	}
}


YOCTO_UNIT_TEST_IMPL(crew)
{
	std::cerr << "Testing Crew Create/Destroy" << std::endl;
	for( size_t i=0; i <= 3; ++i )
	{
		threading::crew SIMD( 1 << i );
	}
	
	
	// num procs
	size_t np = 1;
	if( argc > 1 )
	{
		np = atol( argv[1] );
	}
	std::cerr << "-- num threads = " << np << std::endl;
	
	
	// num items
	size_t width = 1000;
	if( argc > 2 )
	{
		width = size_t(strtod(argv[2], NULL));
	}
	std::cerr << "-- witdh = " << width << std::endl;
	
	// num iter
	size_t nr = 2;
	if( argc > 3 )
	{
		nr = size_t(strtod(argv[3], NULL));
	}
	std::cerr << "-- num loops=" << nr << std::endl;
	
	
	// create the crew
	std::cerr << "-- creating the working crew!" << std::endl;
	threading::crew SIMD( np );
	
	// slice the data width
	std::cerr << "-- slicing data" << std::endl;
	buffer_of<slice_t,global> slices( SIMD.size );
	{
		size_t offset = 0;
		size_t remain = width;
		for( size_t r=0; r < SIMD.size; ++r )
		{
			const size_t length = remain/(SIMD.size - r );
			slices[r].offset = offset;
			slices[r].length = length;
			std::cerr << "slices " << offset << "+" << length << std::endl;
			offset += length;
			remain -= length;
		}
	}
	
	// task to be done
	SIMD.proc = func;
	
	// dispatch args to be processed
	SIMD.set_args( slices(0) );
	
	std::cerr << "-- run " << std::endl;
	wtime chrono;
	double ell = 0;
	for( size_t j=0; j < nr; ++j )
	{
		chrono.start();
		SIMD.run();
		double tot = 0;
		for( size_t i=0; i < SIMD.size; ++i )
		{
			tot += slices[i].result;
		}
		tot = sqrt( 6.0 * tot );
		ell += chrono.query();
		std::cerr << "ans=" << tot << std::endl;
	}
	ell *= 1000;
	if( ell > 0 ) ell /= nr;
	//const double ell = chrono.query();
	std::cerr << "-- done in " << ell << " ms/run" << std::endl;
	std::cerr << "-- end " << std::endl;
}
YOCTO_UNIT_TEST_DONE()
