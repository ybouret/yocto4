#include "yocto/hashing/digest.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/hashing/function.hpp"


namespace yocto
{
	
	
	
	digest digest::get_from( hashing::function &h )
	{
		digest ans( h.length, 0 );
		h.get( ans.buffer_, ans.length_ );
		return ans;
	}
	
	digest digest::checksum( hashing::function &h, const ro_buffer &b )
	{
		h.set();
		h(b);
		return digest::get_from(h);
	}
	
	static
	inline unsigned int x2b( const char C )
	{
		
		int ans = hex2dec(C);
		if(ans<0)
			throw exception("digest::hex[invalid char '%c']",C);
		
		return unsigned(ans);
	}
	
	
	digest digest:: hex( const string &s )
	{
		const string hx = (s.size() & 1) ? ("0" + s) : (s);
		assert( ( hx.size() & 1)  == 0 );
		const size_t hn = hx.size() >> 1;
		digest d( hn, 0 );
		
		uint8_t    *W = (uint8_t *) & d[0];
		const char *C = &hx[0];
		for( size_t i=hn; i>0; --i, ++W, C+=2 )
		{
			*W = uint8_t( ( x2b( C[0] ) << 4 ) | ( x2b( C[1] ) ) );
		}
		return d;
		
	}

	digest digest:: hex( const char *txt )
	{
		const string s(txt);
		return 	digest::hex(s);
	}
	
	
}
