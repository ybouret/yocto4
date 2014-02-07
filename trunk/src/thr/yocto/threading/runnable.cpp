#include "yocto/threading/runnable.hpp"
#include <new>
#include <cstring>

#if 0
namespace yocto {
	
	namespace threading
	{
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		
		void runnable:: cleanup() throw()
		{
			assert( NULL == thread_ );
			memset( layout_, 0, sizeof(layout_) );
		}
		
		runnable:: runnable() throw() :
		layout_(),
		thread_(NULL)
		{
			cleanup();
		}
		
		runnable:: ~runnable() throw()
		{
			assert( thread_ == NULL );
		}
		
		void runnable:: start()
		{
			assert( NULL == thread_ );
			try
			{
				thread_ = new (layout_) thread( runnable::execute, this );
			}
			catch(...)
			{
				cleanup();
				throw;
			}
		}
		
		void runnable::execute( void *args ) throw()
		{
			assert( args );
			runnable *host = static_cast<runnable *>( args );
			host->run();
		}
		
		void runnable:: join() throw()
		{
			if( thread_ )
			{
				thread_->join();
				destruct( thread_ );
				thread_ = NULL;
				cleanup();
			}
		}
		
		
	}
}
#endif
