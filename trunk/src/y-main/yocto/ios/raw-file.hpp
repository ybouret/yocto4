#ifndef YOCTO_IOS_RAW_FILE_INCLUDED
#define YOCTO_IOS_RAW_FILE_INCLUDED 1

#include "yocto/ios/local-file.hpp"
#include "yocto/ios/file-descriptor.hpp"
#include "yocto/string.hpp"
#include "yocto/error.hpp"

namespace yocto
{
	
	namespace ios
	{
		
		enum
		{
			readable = 0x01,
			writable = 0x02,
			truncate = 0x80
		};
		
		enum whence_t
		{
			from_set,
			from_cur,
			from_end
		};
		
		typedef int64_t offset_t;
		
		class raw_file : public local_file
		{
		public:
			
			inline bool is_readable() const throw() { return 0 != ( access & readable); }
			inline bool is_writable() const throw() { return 0 != ( access & writable); }
			
			
			explicit raw_file( const string &filename, size_t mode );
			explicit raw_file( const cstdin_t  &);
			explicit raw_file( const cstdout_t &);
			explicit raw_file( const cstderr_t &);
			virtual ~raw_file() throw();
			
			typedef file_descriptor::type handle_t;
			
			void get( void *data, size_t size, size_t &done );
			void put( const void *data, size_t size, size_t &done );
			
			offset_t seek( offset_t delta, whence_t whence );
			offset_t tell();
			void     rewind();
			void     unwind();
			size_t   length();
			
			
			void get_all( void *data, size_t size );
			
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
