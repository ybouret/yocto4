#include "yocto/ios/imstream.hpp"
#include "yocto/memory/buffer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	
	namespace ios
	{
		
		imstream:: ~imstream() throw() {}
		
		imstream:: imstream( const void *buffer, size_t buflen ) throw() :
		curr_( (const char *)buffer ),
		last_( curr_ + buflen ),
		first_( curr_ )
		{
			assert( !(buffer==NULL && buflen>0 ) );
		}
		
		imstream:: imstream( const memory::ro_buffer &buffer ) throw() :
		curr_( (const char *) buffer.ro() ),
		last_( curr_ + buffer.length() ),
		first_( curr_ )
		{
		}
		
		bool imstream:: query( char &C )
		{
			if( curr_ < last_ )
			{
				C = *(curr_)++;
				return true;
			}
			else {
				return false;
			}

		}
		
		void imstream:: store( char C ) 
		{
			if( curr_ <= first_ || C != curr_[-1] )
				throw exception("invalid imstream::store(#%d)", int(C));
			--curr_;
		}
		
	}
	
}

