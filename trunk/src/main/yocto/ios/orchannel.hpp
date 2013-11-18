#ifndef YOCTO_IOS_ORCHANNEL_INCLUDED
#define YOCTO_IOS_ORCHANNEL_INCLUDED 1

#include "yocto/ios/ochannel.hpp"
#include "yocto/ios/raw-channel.hpp"

namespace yocto
{
	namespace ios
	{
		
		class orchannel : public ochannel, public raw_channel
		{
		public:
			explicit orchannel( const string &filename, offset_t at=0, error_type *status=NULL );
            explicit orchannel( const char   *filename, offset_t at=0, error_type *status=NULL );
            explicit orchannel( raw_file::handle_t handle, error_type *status = NULL); //!< for pipe
			virtual ~orchannel() throw();
			virtual void put( const void *data, size_t size, size_t &done );
						
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(orchannel);   
		};
	}
	
}

#endif
