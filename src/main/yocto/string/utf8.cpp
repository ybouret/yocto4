#include "yocto/string/utf8.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/unroll.hpp"
#include <cerrno>

namespace yocto
{
#define Y_UTF8_SUB "[\\x80-\\xBF]"
	
	const char UTF8::ValidCharRX[]    = "[ !\\x23-\\x5B\\x5D-\\x7F]";
	const char UTF8::Valid8to11RX[]   = "[\\xC2-\\xDF]" Y_UTF8_SUB ;
	const char UTF8::Valid12to16RX[]  = "[\\xE0-\\xEF]" Y_UTF8_SUB Y_UTF8_SUB ;
	const char UTF8::Valid17to21RX[]  = "[\\xF0-\\xF7]" Y_UTF8_SUB Y_UTF8_SUB Y_UTF8_SUB;
	
	bool UTF8::Encode( utf8_t U, string &out )
	{
		
		if( U < (1<<7) )
		{
			//-- 0..7 bits
			out.append( char( U ) );
		}
		else
		{
			//-- 8..11 bits => 5+6
			if( U < (1<<11) )
			{
				const uint32_t U1 = 0xC0 | ( (U >> 6)  & 0x1F ); //!< 5 upper  bits
				const uint32_t U0 = 0x80 | ( U         & 0x3F ); //!< 6 lower bits
				out.append( char( U1 ) );
				out.append( char( U0 ) );
			}
			else
			{
				// 12..16 bits => 4 + 6 + 6
				if( U < (1<<16) )
				{
					const uint32_t U2 = 0xE0 | (  ( U >> 12 ) & 0x0F ); //!< 4 upper bits
					const uint32_t U1 = 0x80 | (  ( U >> 6  ) & 0x3F ); //!< 6 middle bits
					const uint32_t U0 = 0x80 | (    U         & 0x3F ); //!< 6 lower bits
					out.append( char( U2 ) );
					out.append( char( U1 ) );
					out.append( char( U0 ) );
				}
				else
				{
					// 17..21 bits => 3 + 6 + 6 + 6
					if( U < (1<<21) )
					{
						const uint32_t U3 = 0xF0 | (  ( U >> 18 ) & 0x07 ); //!< 3 upper bits
						const uint32_t U2 = 0x80 | (  ( U >> 12 ) & 0x3F ); //!< 6 next
						const uint32_t U1 = 0x80 | (  ( U >>  6 ) & 0x3F ); //!< 6 next
						const uint32_t U0 = 0x80 | (    U         & 0x3F ); //!< 6 lower bits
						out.append( char( U3 ) );
						out.append( char( U2 ) );
						out.append( char( U1 ) );
						out.append( char( U0 ) );
					}
					else
					{
						//throw libc::exception( ERANGE, "UTF8 code point 0x%08x too big", (unsigned) U );
                        return false;
					}
				}
			}
		}
        return true;
	}
	
	static inline
	utf8_t Decode6( uint8_t X, const char *msg )
	{
		if( 0x80 != ( X & 0xC0 ) )
			throw exception("[UTF8: invalid sub-byte 0x%02x (%s)]", unsigned(X), msg );
		return X & 0x3F;
	}
	
	utf8_t UTF8::Decode( const char * &curr, const char *last )
	{
		
		if( curr >= last )
			throw exception("[UTF8: No Byte]" );
		
		const uint8_t a = *(curr++);
		if( a & 0x80 )
		{
			
			if( 0xC0 == (a & 0xE0 ) )
			{
				// 0x110xxxxx
				utf8_t U = a & 0x1F;
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 2/2]");
				
				return U | Decode6( *(curr++), "Byte 2/2" );
			}
			
			if( 0xE0 == (a & 0xF0 ) )
			{
				// 0x1110xxxx
				utf8_t U = a & 0x0F;
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 2/3]");
				U |= Decode6( *(curr++), "Byte 2/3" );
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 3/3]");
				U |= Decode6( *(curr++), "Byte 3/3" );
				return U;
			}
			
			if( 0xF0 == ( a & 0xF8 ) )
			{
				// 0x11110xxx
				utf8_t U = a & 0x07;
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 2/4]");
				U |= Decode6( *(curr++), "Byte 2/4" );
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 3/4]");
				U |= Decode6( *(curr++), "Byte 3/4" );
				U <<= 6;
				if( curr >= last ) throw exception("[UTF8: Missing Byte 4/4]");		
				U |= Decode6( *(curr++), "Byte 4/4" );
				return U;
				
			}
			
			throw exception("[UTF8: Invalid Header Byte 0x%02x]", unsigned(a) );
			return 0;
		}
		else
			//---------------
			// a = 0x0xxxxxxx
			//---------------
			return a;
	}
	
	string UTF8:: Encode( const wstring & ws )
	{
		string ans;
		const utf8_t *W = ws.c_str();
		YOCTO_LOOP(ws.size(), if(!UTF8::Encode( *(W++), ans )) throw exception("invalid UTF8 string") );
		return ans;
	}
	
	wstring UTF8:: Decode( const string & s)
	{
		wstring ans;
		const char *curr = s.c_str();
		const char *last = curr + s.size();
		while( curr < last )
		{
			ans.append( UTF8::Decode( curr, last ) );
		}
		return ans;
	}
	
	
}
