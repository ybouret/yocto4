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
		double  sum;
	};
	
	static 
	void thread_proc( void *args )
	{
		thread_data       &d = *(thread_data *)args;
		mutex             &m = *d.synchro;
		for( size_t i=0; i < 10; ++i)
		{
			wtime::sleep( alea<double>() * 0.1 );
			YOCTO_LOCK(m);
			std::cerr << "\tId=" << thread::get_current_id() << std::endl;
			d.sum += 1;
			std::cerr << "\tsum=" << d.sum << std::endl;
		}
	}
}

YOCTO_UNIT_TEST_IMPL(thread)
{
	mutex synchro( "synchro" );
	thread_data d = { &synchro, 0 };
	
	thread thr1( thread_proc, &d );
	thread thr2( thread_proc, &d );
	thread thr3( thread_proc, &d );

	thr1.join();
	thr2.join();
	thr3.join();
	std::cerr << "Final sum=" << d.sum << std::endl;
}
YOCTO_UNIT_TEST_DONE()

#include <cstdio>
#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(place)
{
	size_t cpu_id = 0;
	if( argc > 1 ) cpu_id = atoi(argv[1]);
	std::cerr << "Placing main thread on CPU #" << cpu_id << std::endl;
    thread::assign_cpu( thread::get_current_handle(), cpu_id);
	std::cerr << "Press Enter after check!" << std::endl;
	fgetc(stdin);
}
YOCTO_UNIT_TEST_DONE()

