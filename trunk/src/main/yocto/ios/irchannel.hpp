#ifndef YOCTO_IOS_IRCHANNEL_INCLUDED
#define YOCTO_IOS_IRCHANNEL_INCLUDED 1

#include "yocto/ios/ichannel.hpp"
#include "yocto/ios/raw-channel.hpp"

namespace yocto
{
	namespace ios
	{
		
		class irchannel : public ichannel, public raw_channel
		{
		public:
			explicit irchannel( const string &filename, offset_t at=0, error_type *status=NULL );
            explicit irchannel( const char   *filename, offset_t at=0, error_type *status=NULL );
            explicit irchannel( raw_file::handle_t handle, error_type *status = NULL); //!< for pipe
			virtual ~irchannel() throw();
			virtual size_t get( void *data, size_t size );
						
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(irchannel);			
		};
	}
	
}

#endif
