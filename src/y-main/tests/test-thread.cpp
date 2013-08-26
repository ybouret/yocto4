#include "yocto/threading/proxy.hpp"
#include "yocto/sys/wtime.hpp"
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
#include <cmath>


static void cpu_proc( size_t cpu_id, void * )
{
	std::cerr << "\tOn CPU #" << cpu_id << std::endl;
}

YOCTO_UNIT_TEST_IMPL(place)
{
	size_t cpu_id = 0;
	if( argc > 1 ) cpu_id = atoi(argv[1]);
	std::cerr << "Placing main thread on CPU #" << cpu_id << std::endl;
	std::cerr << "Before Placement: " << std::endl;
	thread::foreach_cpu( thread::get_current_handle(), cpu_proc, 0 );
	thread::assign_cpu( thread::get_current_handle(), cpu_id);
	std::cerr << "After Placement:  " << std::endl;
	thread::foreach_cpu( thread::get_current_handle(), cpu_proc, 0 );
    
#if 0
	std::cerr << "Press Enter after check!" << std::endl;
	fgetc(stdin);
#endif
	std::cerr << "Computing..." << std::endl;
	wtime chrono;
	double res = 0;
	chrono.start();
	double t_last = -1;
	for(size_t i=1; ; ++i)
	{
		const double tmp = double(i);
		res += 1.0/(tmp*tmp);
		const double t_now = chrono.query();
		if( t_now - t_last >= 0.5 )
		{
			(std::cerr << sqrt(6*res) << std::endl).flush();
			t_last = t_now;
		}
		if( chrono.query() >= 5.0 )
			break;
	}
}
YOCTO_UNIT_TEST_DONE()


namespace {
    
    static void do_something()
    {
        std::cerr << "doing something in thread" << std::endl;
    }
    
    struct run_something
    {
        void operator()(void)
        {
            std::cerr << "run something()" << std::endl;
        }
        
        void compute()
        {
            std::cerr << "run compute()" << std::endl;
        }
        
    };
    
    void do_display( int a )
    {
        std::cerr << "display: " << a << std::endl;
    }
    
    
}

#include "yocto/functor.hpp"

YOCTO_UNIT_TEST_IMPL(thr_proxy)
{
    threading::proxy thr;
    thr.trigger( do_something );
    thr.finish();

    run_something run;
    thr.trigger(run);
    thr.finish();
    
    functor<void,null_type> fn( &run , & run_something::compute );
    thr.trigger(fn);
    thr.finish();
    
    int a = 7;
    thr.trigger(do_display, a);
    thr.finish();
}
YOCTO_UNIT_TEST_DONE()

