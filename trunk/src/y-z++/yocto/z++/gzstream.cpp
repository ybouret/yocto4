#include "yocto/z++/gzstream.hpp"

#include "yocto/zlib/zlib.h"
#include <cerrno>
#include "yocto/exceptions.hpp"
#include "yocto/memory/global.hpp"

//#include <iostream>

namespace yocto
{

	namespace zlib
	{

		gzstream:: ~gzstream() throw()
		{
			YOCTO_GIANT_LOCK();
			(void) gzclose( (gzFile)GZ );
		}

		gzstream:: gzstream( void * __GZ ) throw() :
		GZ( __GZ )
		{

		}

	}

	namespace ios
	{

		igzstream::  ~igzstream() throw() {}

		static inline void * __input_gz( const string &file_name )
		{
			YOCTO_GIANT_LOCK();
			void *h = gzopen( &file_name[0], "rb" );
			if( !h )
			{
				const int err = errno;
				throw libc::exception( err, "gzopen(%s)", &file_name[0] );
			}

			return h;
		}
		
		static inline void * __input_gz( const char *file_name )
		{
			const string fn(file_name);
			return __input_gz(fn);
		}

		igzstream:: igzstream( const string & file_name ) :
		zlib::gzstream( __input_gz( file_name  ) )
		{
		}

		igzstream:: igzstream( const char * file_name ) :
                zlib::gzstream( __input_gz( file_name  ) )
                {
                }
	
		bool igzstream:: query( char &C )
		{
			const int res = gzgetc( (gzFile) GZ );
			if( res < 0 )
				return false;
			else
			{
				C = char(res);
				return true;    
			}
		}

		void igzstream:: store( char C)
		{
			const int ch  = uint8_t(C);
			const int res = gzungetc( ch, (gzFile)GZ );
			if( res < 0 )
			{
				throw exception("gzungetc FAILURE");
			}
		}


		ogzstream::  ~ogzstream() throw() {}

		static inline void * __output_gz( const string &file_name, size_t level , bool append )
		{
			YOCTO_GIANT_LOCK();
			const char mode[4] = { append ? 'a': 'w', 'b', char('0' + ( level % 10 )), '\0' };
			//std::cerr << "gz mode=" << mode << std::endl;
			void *h = gzopen( &file_name[0], mode );
			if( !h )
			{
				const int err = errno;
				throw libc::exception( err, "gzopen(%s,%s)", &file_name[0], mode);
			}

			return h;
		}

		static inline void * __output_gz( const char *file_name, size_t level, bool append )
		{
			const string fn(file_name);
			return __output_gz(fn,level,append);
		}
	
		ogzstream:: ogzstream( const string & file_name, size_t level , bool append ) :
		zlib::gzstream( __output_gz( file_name, level, append ) )
		{

		}

		ogzstream:: ogzstream( const char * file_name, size_t level , bool append ) :
                zlib::gzstream( __output_gz( file_name, level, append ) )
                {

                }

		void ogzstream:: write( char C )
		{
			assert( GZ );
			if( gzwrite( (gzFile) GZ, &C,1 ) <= 0 )
				throw exception( "gzwrite()" );
		}

		void ogzstream:: flush()
		{
			if( gzflush( (gzFile) GZ, Z_FINISH ) < 0 )
			{
				throw exception( "gzflush" );
			}
		}


	}


}
