#include "yocto/mpk/natural.hpp"

#include "yocto/code/binary.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include <iostream>
#include <cstring>

namespace yocto
{
	
	namespace mpk
	{
		
		
		const uint8_t natural:: _bit[8] =
		{
			0x01,
			0x02,
			0x04,
			0x08,
			0x10,
			0x20,
			0x40,
			0x80
		};
		
		const uint8_t natural:: _msk[8] =
		{
			0xff - 0x01,
			0xff - 0x02,
			0xff - 0x04,
			0xff - 0x08,
			0xff - 0x10,
			0xff - 0x20,
			0xff - 0x40,
			0xff - 0x80
		};
		
#define YMPK_CHECK(MP) \
/* */ assert( (MP)->maxi_ > 0 ); \
/* */ assert( (MP)->byte_ != NULL ); \
/* */ assert( (MP)->size_ <= (MP)->maxi_ ); \
/* */ assert( 0 == (MP)->size_ || (MP)->byte_[ (MP)->size_ - 1] != 0 )
		
		natural:: ~natural() throw()
		{
			YMPK_CHECK(this);
			mem_release( byte_, maxi_ );
		}
		
		natural:: natural() :
		size_(0),
		maxi_(0),
		byte_( mem_acquire(maxi_) )
		{
			YMPK_CHECK(this);
		}
		
		size_t natural:: bits() const throw()
		{
			if( size_ > 0 )
			{
				const size_t   S = size_-1;
				const unsigned B = byte_[S]; assert( B > 0 );
				return (S << 3 ) + ceil_ln2_table[B];			
			}
			else 
			{
				return 0;
			}
			
		}
		
		
		void natural:: update() throw()
		{
			const uint8_t *B = byte_+size_;
			while( size_ > 0 && *(--B) <= 0 ) --size_;
			
			YMPK_CHECK(this);
			
		}
		
		void natural:: rescan() throw()
		{
			size_ = maxi_;
			update();
		}
		
		void natural:: ldz() throw()
		{
			YMPK_CHECK(this);
			size_ = 0;
			YMPK_CHECK(this);
		}
        
		void natural:: ldZ() throw()
		{
			YMPK_CHECK(this);
			size_ = 0;
			memset(byte_,0,maxi_);
			YMPK_CHECK(this);
		}
		
		bool natural:: is_zero() const throw()
		{
			YMPK_CHECK(this);
			return size_ <= 0;
		}
		
		bool natural:: is_one() const throw()
		{
			YMPK_CHECK(this);
			return size_ == 1 && byte_[0] == 1;
		}
		
		bool natural:: is_two() const throw()
		{
			YMPK_CHECK(this);
			return size_ == 1 && byte_[0] == 2;
		}
		
		
		bool natural:: is_positive() const throw()
		{
			YMPK_CHECK(this);
			if( size_ > 0 )
			{
				assert( byte_[size_-1] > 0 );
				return true;
			}
			else
				return false;
		}
		
		
		
		natural:: natural( const natural &other ) :
		size_( other.size_ ),
		maxi_( other.size_ ),
		byte_( mem_acquire(maxi_) )
		{
#			define YMPK_CPY(J) byte_[J] = other.byte_[J]
			YOCTO_LOOP_FUNC(size_,YMPK_CPY,0);
		}
		
		void natural:: xch( natural &other ) throw()
		{
			cswap( size_, other.size_ );
			cswap( maxi_, other.maxi_ );
			cswap( byte_, other.byte_ );
		}
		
		natural & natural:: operator=( const natural & other )
		{
			natural tmp( other );
			xch( tmp );
			return *this;
		}
		
		size_t       natural:: length() const throw() { return size_; }
		const void * natural:: get_address() const throw() { return byte_; }
		
		natural:: natural( const ro_buffer &buf ) :
		size_( buf.length() ),
		maxi_( size_ ),
		byte_( mem_acquire( maxi_ ) )
		{
			const uint8_t *from = (uint8_t*)buf.ro();
#			define YMPK_FROM(J) byte_[J] = from[J]
			YOCTO_LOOP_FUNC(size_,YMPK_FROM,0);
			update();
		}
		
#if 0
		natural:: natural( const uint8_t &x ) :
		size_( 1 ),
		maxi_( size_ ),
		byte_( mem_acquire(maxi_) )
		{
			byte_[ 0 ] = x;
			update();
		}
		
		natural:: natural( const uint16_t &x ) :
		size_( 2 ),
		maxi_( size_ ),
		byte_( mem_acquire(maxi_) )
		{
			byte_[ 0 ] =  uint8_t( (x)    & 0xff );
			byte_[ 1 ] =  uint8_t( (x>>8) & 0xff );
			update();
		}
		
		natural:: natural( const uint32_t &x ) :
		size_( 4 ),
		maxi_( size_ ),
		byte_( mem_acquire(maxi_) )
		{
			byte_[ 0 ] =  uint8_t( (x)     & 0xff );
			byte_[ 1 ] =  uint8_t( (x>>8)  & 0xff );
			byte_[ 2 ] =  uint8_t( (x>>16) & 0xff );
			byte_[ 3 ] =  uint8_t( (x>>24) & 0xff );
			update();
		}
		
		natural:: natural( const uint64_t &x ) :
		size_( 8 ),
		maxi_( size_ ),
		byte_( mem_acquire(maxi_) )
		{
			byte_[ 0 ] =  uint8_t( (x)     & 0xff );
			byte_[ 1 ] =  uint8_t( (x>>8)  & 0xff );
			byte_[ 2 ] =  uint8_t( (x>>16) & 0xff );
			byte_[ 3 ] =  uint8_t( (x>>24) & 0xff );
			byte_[ 4 ] =  uint8_t( (x>>32) & 0xff );
			byte_[ 5 ] =  uint8_t( (x>>40) & 0xff );
			byte_[ 6 ] =  uint8_t( (x>>48) & 0xff );
			byte_[ 7 ] =  uint8_t( (x>>56) & 0xff );
			update();
		}
#endif
        
        natural:: natural( const uint64_t x ) :
		size_( 8 ),
		maxi_( size_ ),
		byte_( mem_acquire(maxi_) )
		{
			byte_[ 0 ] =  uint8_t( (x)     & 0xff );
			byte_[ 1 ] =  uint8_t( (x>>8)  & 0xff );
			byte_[ 2 ] =  uint8_t( (x>>16) & 0xff );
			byte_[ 3 ] =  uint8_t( (x>>24) & 0xff );
			byte_[ 4 ] =  uint8_t( (x>>32) & 0xff );
			byte_[ 5 ] =  uint8_t( (x>>40) & 0xff );
			byte_[ 6 ] =  uint8_t( (x>>48) & 0xff );
			byte_[ 7 ] =  uint8_t( (x>>56) & 0xff );
			update();
		}
        
		natural:: natural( size_t n, const as_capacity_t & ) :
		size_(n),
		maxi_(n),
		byte_( mem_acquire(maxi_) )
		{
		}
		
		natural:: natural( const void *buffer, const size_t buflen ) :
		size_( buflen ),
		maxi_( buflen ),
		byte_( mem_acquire(maxi_) )
		{
			assert( !(buffer==NULL && buflen>0) );
			const uint8_t *from = (const uint8_t *)buffer;
			YOCTO_LOOP_FUNC(size_,YMPK_FROM,0);
			update();
			
		}
		
		
		natural natural:: one()
		{
			return natural( 1 );
		}
		
		natural natural:: two()
		{
			return natural( 2 );
		}
		
		bool natural:: is_even() const throw() 
		{
			return (byte_[0] & 0x01) == 0;
		}
		
		bool natural:: is_odd() const throw()
		{
			return (byte_[0] & 0x01) != 0;
		}
		
        
        natural natural::dec( const string &str )
        {
            natural res;
            const natural Ten( uint8_t(10) );
            for( size_t i=0; i < str.size(); ++i )
            {
                res *= Ten;
                const char C = str[i];
                if( C >= '0' && C <= '9' )
                {
                    const natural X( uint8_t( C - '0' ) );
                    res += X;
                }
                else
                    throw exception("natural.dec(invalid '%c')", C );
            }
            return res;
        }
        
        
        natural natural:: dec( const char *txt )
        {
            const string str(txt);
            return dec( str );
        }
        
        natural natural::hex( const string &str )
        {
            natural res;
            const natural Sixteen( uint8_t(16) );
            for( size_t i=0; i < str.size(); ++i )
            {
                res *= Sixteen;
                const char C = str[i];
                if( C >= '0' && C <= '9' )
                {
                    const natural X( uint8_t( C - '0' ) );
                    res += X;
                    continue;
                }
                
                if( C >= 'a' && C <= 'f' )
                {
                    const natural X( uint8_t( 10 + (C - 'a' ) ) );
                    res += X;
                    continue;
                }
                
                if( C >= 'A' && C <= 'F' )
                {
                    const natural X( uint8_t( 10 + (C - 'A' ) ) );
                    res += X;
                    continue;
                }
                
                
                throw exception("natural.hex(invalid '%c')", C );
            }
            return res;
        }
        
        
        natural natural:: hex( const char *txt )
        {
            const string str(txt);
            return hex( str );
        }
        
    }
    
    
}

#include <iostream>
namespace yocto
{
	namespace mpk
	{
		void natural:: out( std::ostream &os ) const
		{
			YMPK_CHECK(this);
			if( is_zero() )
			{
				os << '0';
			}
			else 
			{
				assert( is_positive() );
                
                if( os.flags() & std::ios_base::hex )
				{
                    //----------------------------------------------------------
					// hexa decimal format
                    //----------------------------------------------------------
					assert(size_>0);
					const uint8_t *p = byte_ + size_;
					
					//-- first byte
					{
						const unsigned B = *(--p); assert( B!=0 );
						const char hi = hexa_char[ (B>>4) & 0xf ];
						if( hi != '0' ) os << hi;
						const char lo = hexa_char[ ( B  ) & 0xf ];
						os << lo;
					}
					
					//-- last bytes
					for( size_t i=size_-1;i>0;--i)
					{
						const unsigned B = *(--p);
						os << hexa_char[ (B>>4) & 0xf ];
						os << hexa_char[ ( B  ) & 0xf ];
					}
				}
                else
                {
                    //----------------------------------------------------------
					// decimal format
                    //----------------------------------------------------------
                    string ans;
                    const natural Ten( uint8_t(10) );
                    natural       Arg = *this;
                    while( Arg >= Ten )
                    {
                        const natural Quot = Arg/Ten;
                        const natural Rem  = Arg - Ten * Quot;
                        assert(Rem<Ten);
                        ans.append( char('0' + Rem.byte_[0]) );
                        Arg = Quot;
                    }
                    ans.append(char('0' + Arg.byte_[0]));
                    for( size_t i=ans.size();i>0;--i)
                    {
                        os << ans[i-1];
                    }
                }
			}
			
			
		}
	}
}

#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
	namespace mpk
	{
		void natural:: save( ios::ostream &out ) const
        {
            out.emit<uint32_t>( size_ );
            out <<  *this;
        }

        natural natural:: load( ios::istream &fp )
        {
            const uint32_t len = fp.read<uint32_t>();
            natural        ans( len, as_capacity );
            fp.load( ans.byte_, len );
            ans.rescan();
            return ans;
        }
        
    }
    
}


