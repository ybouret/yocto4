#include "yocto/ios/iflux.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>



namespace yocto
{
	namespace ios
	{
		iflux:: ~iflux() throw()
		{}
		
		iflux:: iflux( ios::ichannel &ch, memory::rw_buffer &buff )  :
		in_( ch ),
		init_( static_cast<char *>( buff.rw() ) ),
		curr_( init_ ),
		size_( buff.length() ),
		last_( init_  )
		{
			if( size_ <= 0 ) throw libc:: exception( ENOMEM, "iflux(empty buffer)" );
		}
		
		bool iflux:: query( char &C )
		{
			if( curr_ >= last_ && ! fetch() )
				return false;
			else 
			{
				C = *(curr_++);
				return true;
			}
			
		}
		
		bool iflux:: fetch()
		{
			last_ = curr_ = init_;
			size_t  done  = 0;
			in_.get( init_, size_, done );
			if( done > 0 )
			{
				last_ = init_ + done;
				return true;
			}
			else
				return false;
		}
		
		void iflux:: store( char C )
		{
			if( curr_ <= init_ )
				throw exception( "iflux.store(IMPOSSIBLE)" );
			*(--curr_) = C;
		}
		
	}
	
}

