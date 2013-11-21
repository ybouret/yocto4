#include "yocto/ios/istream.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
	
	namespace ios
	{
		
		istream:: ~istream() throw() {}
		istream:: istream() throw()  {}
		
        char istream:: read_char(const char *field)
        {
            const char *msg = field ? field : "input byte";
            char C = 0;
            if( !query(C) )
                throw libc::exception( EIO, "istream::read_char(%s)", msg);
            return C;
        }
        
        uint8_t istream:: read_byte(const char *field)
        {
            const char *msg = field ? field : "input byte";
            char C = 0;
            if( !query(C) )
                throw libc::exception( EIO, "istream::read_byte(%s)", msg);
            return uint8_t(C);
        }
        
		void istream:: get( void *data, size_t size, size_t &done )
		{
			assert( !(data==NULL&&size>0) );
			char  *C = static_cast<char*> (data);
			done = 0;
			while( done < size )
			{
				if( !query( *(C++) ) ) return;
				++done;
			}
		}
		
		void istream:: load( void *buffer, size_t buflen )
		{
			size_t loaded = 0;
			get(buffer,buflen,loaded);
			if(  loaded < buflen )
				throw libc::exception( EIO, "istream::load( %u < %u )", unsigned(loaded), unsigned(buflen));
		}
		
		int istream:: read_line( string &line ) { return read_line::scan( *this, line ); }
		
#if defined(__GNUC__)
		const int read_line::failure;
		const int read_line::partial;
#endif
		
		int read_line:: scan(  istream &in, string &line )
		{
			static const int CR( '\r' );
			static const int LF( '\n' );
			
			const char  *org = (const char *)line.ro();
			const size_t len = line.length();
			const char  *end = org + len;
			
			int prev = -1;
			int last = -1;
			switch( len )
			{
				case 0:
					break;
					
				case 1:
					last = end[-1];
					break;
					
				default:
					assert( len >= 2 );
					prev = end[-2];
					last = end[-1];
					break;
			}
			
			char next(0);
		TEST_STATUS:
			switch( prev )
			{
					//--------------------------------------------------------------
					// CR + ?
					//--------------------------------------------------------------
				case CR:
					if( last == LF )
					{
						line.trim(2);
						return read_line:: found_crlf; //-- line is ready !
					}
					else
					{
						in.store( char(last) );
						line.trim(1);
						return read_line:: found_cr; //-- line is ready !
					}
					
					//--------------------------------------------------------------
					// LF + ? => cut already !
					//--------------------------------------------------------------
				case LF:
					in.store( char(last) );
					line.trim(1);
					return read_line:: found_cr; //-- line is ready !
					break;
					
					
					//--------------------------------------------------------------
					// pref is not CR/LF: what about last ?
					//--------------------------------------------------------------
				default:
					switch( last )
				{
						//------------------------------------------------------
						// xxxx[LF] => fast !
						//------------------------------------------------------
					case LF:
						line.trim(1);
						return read_line:: found_lf; //-- line is ready !
						//break;
						
						//------------------------------------------------------
						// xxxx[CR] => try to go forward
						//------------------------------------------------------
					case CR:
						if( !in.query(next) )
							return read_line:: hangs_on_cr; //-- line is NOT ready !
						else
						{
							line.trim(1); // remove CR
							if( LF == ptrdiff_t(next) )
							{
								// early return !
								return read_line:: found_crlf; //-- line is ready
							}
							else
							{
								in.store( next );            //-- for next line
								return read_line:: found_cr;  //-- this line is ready
							}
						}
						//break;
						
					default: //-- forward
						if( !in.query( next ) )
							goto EOS;
						else
						{
							line.append( next );
							prev = last;
							last = next;
							goto TEST_STATUS;
						}
						
				}
					
			}
			
		EOS:
			return line.size() > 0 ? (read_line:: partial) : (read_line::failure);
		}
		
		
	}
	
}

