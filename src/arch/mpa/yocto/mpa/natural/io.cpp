#include "yocto/mpa/natural.hpp"
#include "yocto/code/utils.hpp"
#include <iostream>

namespace yocto
{
    namespace mpa
    {
        
        void natural:: out( std::ostream &os ) const
        {
            size_t i=size;
            if(i)
            {
                if( os.flags() & std::ios_base::hex )
				{
                    const uint8_t msb = byte[--i];
                    
                    if( msb & 0xf0 )
                        os << hexa_text[msb];
                    else
                        os << hexa_char[msb&0xf];
                    
                    while(i-- > 0)
                    {
                        const uint8_t b = byte[i];
                        os << hexa_text[b];
                    }
                }
                else
                {
                    //----------------------------------------------------------
					// decimal format
                    //----------------------------------------------------------
                    string        ans;
                    const natural Ten( uint8_t(10) );
                    natural       Arg = *this;
                    while( Arg >= Ten )
                    {
                        const natural Quot = Arg/Ten;
                        const natural Rem  = Arg - Ten * Quot;
                        assert(Rem<Ten);
                        ans.append( char('0' + Rem.byte[0]) );
                        Arg = Quot;
                    }
                    ans.append(char('0' + Arg.byte[0]));
                    for( size_t i=ans.size();i>0;--i)
                    {
                        os << ans[i-1];
                    }
                }
            }
            else
            {
                assert( is_zero() );
                os << '0';
            }
        }
        
        std::ostream & operator<<( std::ostream &os, const natural &n )
        {
            n.out(os);
            return os;
        }
    }
    
}

#include "yocto/ios/bitio.hpp"

namespace yocto
{
    namespace mpa
    {
        
        void natural:: store(ios::bitio &bio, size_t nbits) const
        {
            size_t count = 0;
			try
			{
				for( ; count < nbits; ++count )
				{
					bio.push( bit(count) );
				}
			}
			catch(...)
			{
				while( count-- > 0 )
					bio.pop_back();
				throw;
			}
        }
        
        void natural:: store(ios::bitio &bio ) const
        {
            store(bio,bits());
        }
        
        natural natural:: query( ios::bitio &in, size_t nbits )
		{
			assert(nbits<=in.size());
			natural ans( YOCTO_ROUND8(nbits) >> 3, as_capacity );
			
			uint8_t *b = ans.byte;
			for( size_t i = 0; i < nbits; ++i )
			{
				if(  in.pop() )
				{
					assert( (i>>3) < ans.maxi );
					b[i>>3] |= (1 << ( i & 7 ));
				}
			}
			
			ans.rescan();
			return ans;
		}
        
        
    }
}

#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
	namespace mpa
	{
		void natural:: save( ios::ostream &out ) const
        {
            out.emit<uint32_t>( size );
            out <<  *this;
        }
        
        natural natural:: load( ios::istream &fp )
        {
            const uint32_t len = fp.read<uint32_t>();
            natural        ans( len, as_capacity );
            fp.load( ans.byte, len );
            ans.rescan();
            return ans;
        }
        
    }
    
}

#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    
    namespace mpa
    {
        natural natural:: hex( const string &s )
        {
            if( s.length() <= 0 ) throw libc::exception( EINVAL, "mpa::hex(empty hexadecimal string)");
            natural ans;
            for(size_t i=0;i<s.length();++i)
            {
                const int v = hex2dec( s[i] );
                if(v<0)
                    throw libc::exception( EINVAL, "mpa::hex(invalid string[%u])", unsigned(i) );
                ans <<= 4;
                const natural a = uint64_t(v);
                ans += a;
            }
            return ans;
        }
        
        natural natural:: hex( const char *txt )
        {
            const string s(txt);
            return hex(s);
        }
        
        natural natural:: dec( const string &s )
        {
            const mpn ten( 10 );
            if( s.length() <= 0 )
                throw libc::exception( EINVAL, "mpa::dec(empty decimal string)");
            natural ans;
            for(size_t i=0;i<s.length();++i)
            {
                ans *= ten;
                const char C = s[i];
                if( C >= '0' && C <= '9')
                {
                    const uint64_t u(C-'0');
                    const natural  a(u);
                    ans += a;
                }
                else
                {
                    throw libc::exception( EINVAL, "mpa::dec(invalid string[%u])", unsigned(i) );
                }
                
            }
            return ans;
        }
        
        natural natural:: dec( const char *txt )
        {
            const string s(txt);
            return dec(s);
        }
        
        natural natural:: parse( const string &s )
        {
            if( s.size() >= 2 && s[0] == '0' && s[1] == 'x' )
                return hex( &s[2] );
            else
                return dec(s);
        }
        
    }
    
}




