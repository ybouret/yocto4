#include "yocto/ios/irchannel.hpp"

namespace yocto
{
	namespace ios
	{
		
		irchannel:: ~irchannel() throw()
		{
			
		}
		
		irchannel:: irchannel( const string &filename, offset_t at, error_type *status ) :
        raw_channel( readable, filename, at, status)
		{
            
		}
        
        irchannel:: irchannel( const char *filename, offset_t at, error_type *status ) :
        raw_channel( readable, filename, at, status)
		{
			
		}
		
		size_t irchannel:: get( void *data, size_t size )
		{
			return h.get( data, size );
		}
		
        
        irchannel:: irchannel( raw_file::handle_t handle, error_type *status) :
        raw_channel( readable, handle, status)
        {
        }
	}
	
}
