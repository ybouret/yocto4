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
		
		void irchannel:: get( void *data, size_t size, size_t &done )
		{
			h.get( data, size, done );
		}
		
        
        irchannel:: irchannel( raw_file::handle_t handle, error_type *status) :
        raw_channel( readable, handle, status)
        {
        }
	}
	
}
