#ifndef YOCTO_IOS_RAW_FILE_INCLUDED
#define YOCTO_IOS_RAW_FILE_INCLUDED 1

#include "yocto/ios/local-file.hpp"
#include "yocto/ios/file-descriptor.hpp"
#include "yocto/code/endian.hpp"
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
            explicit raw_file( const char   *filename, size_t mode );
			explicit raw_file( const cstdin_t  &);
			explicit raw_file( const cstdout_t &);
			explicit raw_file( const cstderr_t &);
			virtual ~raw_file() throw();
			
			typedef _fd::type handle_t;
			
			size_t get( void *data, size_t size );
			void put( const void *data, size_t size, size_t &done );
			
			offset_t seek( offset_t delta, whence_t whence );
			offset_t tell();
			void     rewind();
			void     unwind();
			size_t   length();
			
			
			void get_all( void *data, size_t size );
			void put_all( const void *data, size_t size );
            
            //! big endian read
			template <typename T>
			inline T read() { T x(0); get_all(&x, sizeof(T)); return swap_be_as<T>(x);}
            
            //! big endian emit
            template <typename T>
			inline void emit( T x ) { x = swap_be_as<T>(x); put_all(&x,sizeof(T)); } 
            
            void   save_buffer( const memory::ro_buffer &buff ); //!< 32 bits length
            string load_string(); //!< 32 bits length
            
            //! for pipe behavior
            explicit raw_file( handle_t user_handle, size_t user_access ) throw();
            
            //! helper
            static void copy( const string &target, const string &source);
            
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
