#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/ios/ostream.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/chars.hpp"
#include "yocto/type/ints.hpp"
#include "yocto/code/utils.hpp"

#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace yocto
{
	
	namespace ios
	{
		
		ostream:: ~ostream() throw() {}
		ostream:: ostream() throw()  {}
		
		void ostream:: put( const void *data, size_t size, size_t &done )
		{
			assert(!(NULL==data&&size>0) );
			const char *C = (const char *)data;
			for( done=0; done < size; ++done) write( *(C++) );
		}
		
		void ostream:: save( const void *buffer, size_t buflen )
		{
			size_t saved = 0;
			put( buffer, buflen, saved );
			if( saved < buflen )
				throw libc::exception( EIO, "ostream::save(%u < %u)", unsigned(saved), unsigned(buflen) );
		}
        
		void ostream:: append( const char *buffer, size_t buflen )
		{
			while( buflen > 0 )
			{
				size_t done = 0;
				put(buffer,buflen,done);
				buffer += done;
				buflen -= done;
			}
		}
        
        void ostream:: viz( const void *p )
        {
            union
            {
                const void                       *a;
                unsigned_int<sizeof(void*)>::type u;
            }
            alias = { p };
            write('A');
            for(unsigned i=0;i<sizeof(void*);++i)
            {
                const uint8_t b = alias.u & 0xff;
                write( hexa_char[ (b>>4) & 15 ] );
                write( hexa_char[  b     & 15 ] );
                alias.u >>= 8;
            }
        }
        
		
        
        ostream & ostream:: operator<<( const char *buffer)
        {
            append( buffer, length_of(buffer) );
            return *this;
        }
        
        ostream & ostream::operator<<( const memory::ro_buffer &buffer )
        {
            append( (const char *) buffer.ro(), buffer.length() );
            return *this;
        }
        
        ostream & ostream::operator<<( char C )
        {
            write(C);
            return *this;
        }
        
		void ostream:: operator()(const char *fmt,...)
		{
			assert(fmt);
			size_t n = 32;
		TRY:
			{
				memory::buffer_of<char,memory::global> databuf( n );
				char  *buffer  = databuf();
				int    length  = int( databuf.length() );
				if( length < 0 )
					throw libc::exception( ERANGE, "ostream(...) memory overflow");
				
				va_list ap;
				va_start( ap, fmt );
				const int  result  = vsnprintf( buffer, length, fmt, ap);
				const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
				va_end (ap);
				
				if( success )
				{
					append( buffer, length_of(buffer) );
					return;
				}
				n <<= 1;
			}
			goto TRY;
			
		}
		
	}
	
}

