#include "yocto/threading/thread.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/core/list.hpp"

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
			std::cerr << "\tId=" << thread::get_current_handle() << std::endl;
			d.sum += 1;
			std::cerr << "\tsum=" << d.sum << std::endl;
		}
	}
    
}

YOCTO_UNIT_TEST_IMPL(thread)
{
    threading::mutex   mtx("shared");
    thread_data d = { & mtx, 0.0 };
    
    core::list_of<threading::thread> Q;
    for(int i=0;i<3;++i)
    {
        Q.push_back(threading::thread::create_with(mtx));
        Q.tail->launch(thread_proc, &d);
    }
    
    while(Q.size)
    {
        threading::thread *thr = Q.pop_back();
        thr->finish();
        threading::thread::destruct(thr);
    }
    
    std::cerr << "Final Sum=" << d.sum << std::endl;
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
    
    static threading::mutex *shared_access = 0;
    
    static void do_something(void*)
    {
        assert(shared_access);
        YOCTO_LOCK(*shared_access);
        std::cerr << "doing something in thread" << std::endl;
    }
    
    static void do_something0(void)
    {
        assert(shared_access);
        YOCTO_LOCK(*shared_access);
        std::cerr << "doing something0 in thread" << std::endl;
    }
    
    struct run_something
    {
        void operator()(void)
        {
            assert(shared_access);
            YOCTO_LOCK(*shared_access);
            std::cerr << "run something()" << std::endl;
        }
        
        void compute()
        {
            assert(shared_access);
            YOCTO_LOCK(*shared_access);
            std::cerr << "run compute()" << std::endl;
        }
        
    };
    
    static void do_display( int a )
    {
        assert(shared_access);
        YOCTO_LOCK(*shared_access);
        std::cerr << "display: " << a << std::endl;
    }
    
    
}

#include "yocto/functor.hpp"
#include "yocto/threading/threads.hpp"

using namespace threading;
YOCTO_UNIT_TEST_IMPL(threads)
{
    
    threads workers("workers");
    shared_access = & workers.access;
    
    threads::failsafe guard( workers );
    
    workers.launch(do_something,0);
    workers.start(do_something0);
    
    run_something run;
    workers.start(run);
    
    workers.finish();
    
  
    functor<void,null_type> fn( &run , & run_something::compute );
    workers.start(fn);

    int a=7;
    workers.start(do_display,a);
    
    workers.finish();
    
    workers.call( &run, & run_something::compute );
    
}
YOCTO_UNIT_TEST_DONE()


