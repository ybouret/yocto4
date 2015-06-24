#ifndef YOCTO_IOS_OCSTREAM_INCLUDED
#define YOCTO_IOS_OCSTREAM_INCLUDED 1

#include "yocto/ios/ostream.hpp"
#include "yocto/ios/cfile.hpp"
#include "yocto/string.hpp"

#include "yocto/code/printf-check.hpp"

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
			
			virtual void write( char C );
			virtual void flush();
			
			virtual void operator()(const char *fmt,...) YOCTO_PRINTF_CHECK(2,3);
			
            static void overwrite( const string &filename );
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ocstream);
		};


        class acstream : public ocstream
        {
        public:
            virtual ~acstream() throw();
            acstream(const char   *filename);
            acstream(const string &filename);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(acstream);
        };

        class wcstream : public ocstream
        {
        public:
            virtual ~wcstream() throw();
            wcstream(const char   *filename);
            wcstream(const string &filename);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(wcstream);
        };

	}
	
}
#endif
