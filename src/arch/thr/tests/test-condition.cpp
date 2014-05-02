#include "yocto/utest/run.hpp"
#include "yocto/threading/threads.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;

#define NUM_THREADS  3
#define TCOUNT      10
#define COUNT_LIMIT 12


static int                   count = 0;
static threading::mutex     *count_mutex = NULL;
static threading::condition *count_threshold = NULL;


static
void inc_count( void *args ) throw()
{
	const int id = *(int *)args;
	
	for( int i=0; i < TCOUNT; ++i )
	{
		count_mutex->lock();
		++count;
		std::cerr << "inc_count: thread #" << id << " set count=" << count << std::endl;
		
		if( count == COUNT_LIMIT )
		{
			std::cerr << "inc_count:: thread #" << id << " reached threshold!" << std::endl;
			count_threshold->signal();
		}
        std::cerr.flush();
		count_mutex->unlock();
		
		//-- sleep in a while to allow other thread(s) to run
		wtime::sleep( 0.1 * alea<double>() );
    }
}

static
void watch_count( void *args ) throw()
{
    const int id = *(int *)args;
    
    count_mutex->lock();
    std::cerr << "start watch_count: from thread #" << id << std::endl;
    if( count < COUNT_LIMIT )
    {
        //-- unlock mutex and wait
        count_threshold->wait( *count_mutex );
        
        //-- mutex locked after condition is reached
        //-- go, baby
        std::cerr << "watch_count: received condition!" << std::endl;
    }
    count_mutex->unlock();
    
    
}


YOCTO_UNIT_TEST_IMPL(condition)
{
    std::cerr << "Init mutex/condition" << std::endl;
    
    
    int thread_index[] = { 0, 1, 2 };
    threading::threads   workers("counting",3);
    threading::condition count_threshold_;
    
    threading::mutex IO("iomutex");
    count_mutex      = &IO;
    count_threshold  = &count_threshold_;
    
    workers.launch( inc_count,   &thread_index[0] );
    workers.launch( inc_count,   &thread_index[1] );
    workers.launch( watch_count, &thread_index[2] );
    
    
    workers.finish();
}
YOCTO_UNIT_TEST_DONE()

