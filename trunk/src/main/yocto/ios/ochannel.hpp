#ifndef YOCTO_IOS_OCHANNEL_INCLUDED
#define YOCTO_IOS_OCHANNEL_INCLUDED 1

#include "yocto/ios/channel.hpp"

namespace yocto
{

	namespace memory 
	{
		class ro_buffer;
	}
	
	namespace ios
	{
		
		class ochannel : public virtual channel
		{
		public:
			virtual ~ochannel() throw();
			virtual size_t put( const void *data, size_t size ) = 0;
			
			void put_all( const void *data, size_t size, size_t &done );
			void put_all( const memory::ro_buffer &, size_t &done );
			
		protected:
			explicit ochannel() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ochannel);
			
		};
		
	}
	
}

#endif

