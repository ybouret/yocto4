#ifndef YOCTO_IOS_IMSTREAM_INCLUDED
#define YOCTO_IOS_IMSTREAM_INCLUDED 1

#include "yocto/ios/istream.hpp"


namespace yocto
{

	namespace memory
	{
		class ro_buffer;
	}
	
	namespace ios
	{
		class imstream : public istream
		{
		public:
			explicit imstream( const void *buffer, size_t buflen ) throw();
			explicit imstream( const memory::ro_buffer &buffer   ) throw();
			virtual ~imstream() throw();
			
			virtual bool query( char &C );
			virtual void store( char C  );
			
		private:
			const char *curr_;
			const char *last_;
			const char *first_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(imstream);
		};
	}
	
}

#endif
