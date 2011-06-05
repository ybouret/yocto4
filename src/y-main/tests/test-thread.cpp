#include "yocto/threading/thread.hpp"
#include "yocto/wtime.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace threading;

namespace
{
	struct thread_data
	{
		mutex  *synchro;
		int     id;
		double *sum;
	};
	
	static 
	void thread_proc( void *args )
	{
		const thread_data &d = *(thread_data *)args;
		mutex             &m = *d.synchro;
		for( size_t i=0; i < 10; ++i)
		{
			wtime::sleep( alea<double>() * 0.1 );
			YOCTO_LOCK(m);
			std::cerr << "\tId=" << d.id << std::endl;
			*d.sum += 1;
			std::cerr << "\tsum=" << *d.sum << std::endl;
		}
	}
}

YOCTO_UNIT_TEST_IMPL(thread)
{
	mutex synchro( "synchro" );
	double sum = 0;
	thread_data d1 = { &synchro, 1, &sum };
	thread_data d2 = { &synchro, 2, &sum };
	thread_data d3 = { &synchro, 3, &sum };
	
	thread thr1( thread_proc, &d1 );
	thread thr2( thread_proc, &d2 );
	thread thr3( thread_proc, &d3 );

	thr1.join();
	thr2.join();
	thr3.join();
	std::cerr << "Final sum=" << sum << std::endl;
}
YOCTO_UNIT_TEST_DONE()

