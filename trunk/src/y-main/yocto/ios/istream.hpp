#ifndef YOCTO_IOS_ISTREAM_INCLUDED
#define YOCTO_IOS_ISTREAM_INCLUDED 1

#include "yocto/ios/stream.hpp"

namespace yocto
{
	namespace ios 
	{
		class istream : public stream
		{
		public:
			virtual ~istream() throw();
			
			virtual bool query( char &C ) = 0;
			virtual void store( char  C ) = 0;
			
			
			
		protected:
			explicit istream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(istream);
		};
		
	}

}
#endif
