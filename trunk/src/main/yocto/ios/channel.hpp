#ifndef YOCTO_IOS_CHANNEL_INCLUDED
#define YOCTO_IOS_CHANNEL_INCLUDED 1

#include "yocto/object.hpp"

namespace yocto
{

	namespace ios
	{
		
		class channel : public virtual object
		{
		public:
			virtual ~channel() throw();
			
		protected:
			explicit channel() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(channel);
			
		};
		
	}
	
}

#endif
