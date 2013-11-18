#ifndef YOCTO_IOS_OSSTREAM_INCLUDED
#define YOCTO_IOS_OSSTREAM_INCLUDED 1


#include "yocto/ios/ostream.hpp"
#include "yocto/string.hpp"

namespace yocto {
	
	namespace ios
	{
	
		class osstream : public ostream 
		{
		public:
			explicit osstream( string &host ) throw();
			virtual ~osstream() throw();
			
			virtual void write( char C );
			virtual void flush();
			
		private:
			string &host_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(osstream);
		};
		
	}
}
#endif
