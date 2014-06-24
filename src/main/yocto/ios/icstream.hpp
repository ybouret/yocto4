#ifndef YOCTO_IOS_ICSTREAM_INCLUDED
#define YOCTO_IOS_ICSTREAM_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/ios/cfile.hpp"

namespace yocto
{
	namespace ios
	{
		
		class icstream : public istream, public cfile
		{
		public:
			icstream( const string &filename );
			icstream( const char   *filename );
			icstream( const cstdin_t &);
			virtual ~icstream() throw();
			
			virtual bool  query( char &C );
			virtual void   store( char  C );
			virtual size_t get( void *data, size_t size );
			
			bool gets( string &line );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(icstream);
		};
		
		
		
	}
	
}

#endif
