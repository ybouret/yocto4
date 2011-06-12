#ifndef YOCTO_IOS_OCSTREAM_INCLUDED
#define YOCTO_IOS_OCSTREAM_INCLUDED 1

#include "yocto/ios/ostream.hpp"
#include "yocto/ios/cfile.hpp"
#include "yocto/string.hpp"

namespace yocto
{
	namespace ios 
	{
		class ocstream : public ostream, public cfile
		{
		public:
			virtual ~ocstream() throw();
			ocstream( const char   *filename, bool append );
			ocstream( const string &filename, bool append );
			ocstream( const cstdout_t & );
			ocstream( const cstderr_t & );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ocstream);
		};
		
	}
	
}
#endif
