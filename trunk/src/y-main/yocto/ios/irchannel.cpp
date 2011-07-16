#include "yocto/ios/irchannel.hpp"

namespace yocto
{
	namespace ios
	{
		
		irchannel:: ~irchannel() throw()
		{
			
		}
		
		irchannel:: irchannel( const string &filename, offset_t at, error_type *status ) :
		file_( filename, readable )
		{
			file_.status = status;
			file_.seek( at, from_set );
		}
		
		void irchannel:: get( void *data, size_t size, size_t &done )
		{
			file_.get( data, size, done );
		}
		
	}
	
}
