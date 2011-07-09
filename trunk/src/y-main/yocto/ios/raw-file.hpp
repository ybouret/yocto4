#ifndef YOCTO_IOS_RAW_FILE_INCLUDED
#define YOCTO_IOS_RAW_FILE_INCLUDED 1

#include "yocto/ios/local-file.hpp"
#include "yocto/string.hpp"
#include "yocto/error.hpp"

namespace yocto
{
	
	namespace ios
	{
		
		class raw_file : public local_file
		{
		public:
			static const size_t readable = 0x01;
			static const size_t writable = 0x02;
			static const size_t truncate = 0x80;
			
			inline bool is_readable() const throw() { return 0 != ( access & readable); }
			inline bool is_writable() const throw() { return 0 != ( access & writable); }
			
			
			explicit raw_file( const string &filename, size_t mode );
			virtual ~raw_file() throw();
			
#if defined(YOCTO_BSD)
			typedef int handle_t;
#endif
			
#if defined(YOCTO_WIN)
			typedef void *handle_t;
#endif			
			
			
		private:
			handle_t       handle;
			
		public:
			const size_t   access; //!< readable | writable
			error_type    *status; //!< maybe set by user, NULL by default
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(raw_file);
		};
	}
	
}

#endif
