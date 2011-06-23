#ifndef YOCTO_IOS_OCHANNEL_INCLUDED
#define YOCTO_IOS_OCHANNEL_INCLUDED 1

#include "yocto/ios/channel.hpp"

namespace yocto
{
	
	namespace ios
	{
		
		class ochannel : public channel
		{
		public:
			virtual ~ochannel() throw();
			virtual void put( const void *data, size_t size, size_t &done ) = 0;

		protected:
			explicit ochannel() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ochannel);
			
		};
		
	}
	
}

#endif

