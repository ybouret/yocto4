#ifndef YOCTO_IOS_CFILE_INCLUDED
#define YOCTO_IOS_CFILE_INCLUDED 1


#include "yocto/ios/local-file.hpp"
#include <cstdio>

namespace yocto
{

	namespace memory
	{
		class rw_buffer;
	}

	namespace ios
	{
		

		class cfile : public local_file
		{
		public:
			explicit cfile( const char *filename, const char *mode );
			explicit cfile( const cstdin_t  &);
			explicit cfile( const cstdout_t &);
			explicit cfile( const cstderr_t &);
			virtual ~cfile() throw();
			
			void bufferize( memory::rw_buffer &buf );

            FILE * __get() throw();
            
            ptrdiff_t tell() const throw();
            
		protected:
			FILE      *handle;
			YOCTO_DISABLE_COPY_AND_ASSIGN(cfile);
            
		public:
			int *last_close;
		};
	}

}

#endif
