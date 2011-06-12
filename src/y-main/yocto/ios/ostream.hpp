#ifndef YOCTO_IOS_OSTREAM_INCLUDED
#define YOCTO_IOS_OSTREAM_INCLUDED 1

#include "yocto/ios/stream.hpp"

namespace yocto
{
	namespace ios 
	{
		class ostream : public stream
		{
		public:
			virtual ~ostream() throw();
	
			
		protected:
			explicit ostream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ostream);
		};
		
	}
	
}
#endif
