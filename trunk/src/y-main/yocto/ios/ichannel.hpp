#ifndef YOCTO_IOS_ICHANNEL_INCLUDED
#define YOCTO_IOS_ICHANNEL_INCLUDED 1

#include "yocto/ios/channel.hpp"

namespace yocto
{
	
	namespace ios
	{
		
		class ichannel : public virtual channel
		{
		public:
			virtual ~ichannel() throw();
			virtual void get( void *data, size_t size, size_t &done ) = 0;
			
		protected:
			explicit ichannel() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ichannel);
			
		};
		
	}
	
}

#endif
