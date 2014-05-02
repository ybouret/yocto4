#include "yocto/utest/run.hpp"

#include "yocto/threading/barrier.hpp"
#include "yocto/exception.hpp"
#include "yocto/threading/threads.hpp"
#include <iostream>
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/vector.hpp"
#include <cstdlib>

using namespace yocto;

#define THREADS  5
#define ARRAY    64
#define INLOOPS  10000
#define OUTLOOPS 100

namespace {
	
#if defined(_MSC_VER)
	// init of array_
#pragma warning ( disable : 4351 )
#endif
	class computing :  public object
	{
	public:
		explicit computing( threading::barrier &b, size_t nw ) throw() :
		barrier_( b  ),
		number_( nw ),
		array_()
		{
			memset( array_, 0, sizeof(array_) );
		}
		
		virtual ~computing() throw()
		{
		}
		
		static threading::mutex IOmutex;
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN( computing );
		threading::barrier     &barrier_;
		const size_t number_;
		int          array_[ ARRAY ];
		
		static size_t get_rand() throw()
		{
			scoped_lock guard(IOmutex);
			return INLOOPS + ( rand() % (INLOOPS) );
		}
		
    public:
        void run( ) throw()
		{
			/*
			 * Loop through OUTLOOPS barrier cycles.
			 */
			for(size_t out_loop = 0; out_loop < OUTLOOPS; ++out_loop)
			{
				//-- perform common work
				const bool process_status = barrier_.wait();
				{
					scoped_lock guard(IOmutex);
					std::cout << "run [" << out_loop << "]" << "#" << number_ << "/" << THREADS << " <process status=" << process_status << ">" << std::endl;
                }
                /*
                 * This inner loop just adds a value to each element in
                 * the working array.
                 */
                const size_t in_loop_max = get_rand();
                for (size_t in_loop = in_loop_max; in_loop>0; --in_loop )
                {
                    for (size_t count = 0; count < ARRAY; count++)
                    {
                        array_[count] += 1;
                    }
                }
                
                {
                    scoped_lock guard(IOmutex);
                    std::cout << "end [" << out_loop << "]" << "#" << number_ << "/" << THREADS << " -- done " << in_loop_max << std::endl;
                }
                
                //-- shared work, only by one thread
                if( true == barrier_.wait() )
                {
                    scoped_lock guard(IOmutex);
                    std::cout << "--> performing shared update with #" << number_ << "/" << THREADS << std::endl << std::endl;
                }
                
            }
            
        }
        
    };
    
    threading::mutex computing::IOmutex("IOMutex");
    typedef shared_ptr<computing> worker;
    
}

YOCTO_UNIT_TEST_IMPL(barrier)
{
    threading::threads        workers("workers");
    threading::barrier        b( THREADS, "barrier" );
    vector<worker>            w( THREADS, as_capacity );
    
    for( size_t i=1; i <= THREADS; ++i )
    {
        std::cerr <<  "creating #" << i << std::endl;
        worker guy( new computing(b,i) );
        w.push_back( guy );
    }
    
    assert(w.size()==THREADS);
    threading::threads::failsafe guard(workers);
    for(size_t i=1; i <= THREADS; ++i )
    {
        workers.call( & *w[i], & computing::run );
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
