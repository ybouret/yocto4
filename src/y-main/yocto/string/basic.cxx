#include "yocto/string/basic.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>
#include <iostream>

namespace yocto
{
	using namespace memory;
	
	namespace basic
	{
#define YCHECK_STRING(S) \
assert( (S)->full_ > 0 ); \
assert( (S)->char_ != NULL ); \
assert( (S)->maxi_ == (S)->full_ - 1 );\
assert( (S)->size_ <= (S)->maxi_ ); \
assert( (S)->char_[(S)->size_] == 0 )
		
		
		template <>
		std::ostream &string<YCHAR>:: output( std::ostream &os ) const
		{
			os.write( (const char*)char_, size_*sizeof(YCHAR) ); 
			return os;
		}

		template <>
		std::ostream & string<YCHAR>::output_visible( std::ostream &os ) const
		{
			const char    *q = (const char *) char_;
			const size_t   n = size_ * sizeof(YCHAR);
			for( size_t i=0; i < n; ++i )
			{
				os << make_visible( q[i] );
			}
			return os;
		}

		
		template <>
		string<YCHAR>:: ~string() throw()
		{
			YCHECK_STRING(this);
			kind<pooled>::release_as<YCHAR>( char_, full_ );
			maxi_ = 0;
			size_ = 0;
		}
		
		template <>
		string<YCHAR>:: string(  ) :
		size_( 0 ),
		full_( 1 ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			YCHECK_STRING(this);
		}
		
		template <>
		size_t string<YCHAR>:: size() const throw() { YCHECK_STRING(this); return size_; }
		
		template <>
		size_t string<YCHAR>:: capacity() const throw() { YCHECK_STRING(this); return maxi_; }
		
		template <>
		size_t string<YCHAR>:: length() const throw() { YCHECK_STRING(this); return size_ * sizeof(YCHAR); }
		
		template <>
		const void * string<YCHAR>:: get_address() const throw() { YCHECK_STRING(this); return char_; }
		
		template <>
		string<YCHAR>:: string( const string &other ) :
		size_( other.size_ ),
		full_( other.size_ + 1 ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			memcpy( char_, other.char_, size_ * sizeof(YCHAR) );
		}
		
		template <>
		void string<YCHAR>:: swap_with( string &other ) throw()
		{
			YCHECK_STRING(this);
			YCHECK_STRING(&other);
			cswap(full_,other.full_);
			cswap(char_,other.char_);
			cswap(maxi_,other.maxi_);
			cswap(size_,other.size_);
			YCHECK_STRING(this);
			YCHECK_STRING(&other);
		}

		
		//------------------------------------------------------------------
		//
		// constructors
		//
		//------------------------------------------------------------------
		template <>
		string<YCHAR>:: string( size_t n, const as_capacity_t & ) :
		size_(0),
		full_(n+1),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			YCHECK_STRING(this);
		}
		
		template <>
		string<YCHAR>:: string( size_t n, const YCHAR C ) :
		size_(n),
		full_(n+1),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			YCHECK_STRING(this);
			for( size_t i=0; i < size_; ++i ) char_[i] = C;
			YCHECK_STRING(this);
		}
		
		
		
		template <>
		string<YCHAR>:: string( const YCHAR C ) :
		size_(1),
		full_(2),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			char_[0] = C;
			YCHECK_STRING(this);
		}
		
		template <>
		string<YCHAR>:: string( const YCHAR *s ) :
		size_( length_of(s) ),
		full_( 1 + size_    ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			memcpy( char_, s, size_ * sizeof(YCHAR) );
			YCHECK_STRING(this);
		}
		
		template <>
		string<YCHAR>:: string( const YCHAR *s, size_t n) :
		size_( n ),
		full_( 1 + size_    ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			assert( !(n>0 && NULL==s) );
			memcpy( char_, s, size_ * sizeof(YCHAR) );
			YCHECK_STRING(this);
		}
		
		template <>
		string<YCHAR>:: string( size_t sum_len, const YCHAR *a, size_t a_len, const YCHAR *b, size_t b_len ) :
		size_( sum_len  ),
		full_( size_ + 1 ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 )
		{
			assert( a_len + b_len == sum_len );
			memcpy( char_,         a, a_len * sizeof(YCHAR) );
			memcpy( char_ + a_len, b, b_len * sizeof(YCHAR) );
			YCHECK_STRING(this);
		}
		
		template <>
		void string<YCHAR>::clear() throw()
		{
			memset(char_,0,full_*sizeof(YCHAR));
			size_ = 0;
			YCHECK_STRING(this);
		}
		
		//------------------------------------------------------------------
		//
		// assign
		//
		//------------------------------------------------------------------
		template <>
		void string<YCHAR>:: assign( YCHAR C ) throw()
		{
			memset(char_,0,full_*sizeof(YCHAR));
			size_    = 1;
			char_[0] = C;
			YCHECK_STRING(this);
		}
		
		template <>
		void string<YCHAR>:: assign( const YCHAR *s, size_t n )
		{
			assert( !(n>0 && NULL==s) );
			YCHECK_STRING(this);
			if( n <= maxi_ )
			{
				memmove( char_,   s, (size_ = n  ) * sizeof(YCHAR) );
				memset(  char_+n, 0, (full_-size_) * sizeof(YCHAR) );
			}
			else
			{
				string tmp(s);
				swap_with(tmp);
			}
			YCHECK_STRING(this);
		}
		
		//------------------------------------------------------------------
		//
		// append
		//
		//------------------------------------------------------------------
		template <>
		void string<YCHAR>::append( const YCHAR *s, size_t n )
		{
			assert( !(n>0 && NULL==s) );
			YCHECK_STRING(this);
			const size_t sum_len = size_ + n;
			if( sum_len <= maxi_ )
			{
				memmove( char_+size_, s, n * sizeof(YCHAR) );
				size_ = sum_len;
				YCHECK_STRING(this);
			}
			else
			{
				string tmp( sum_len, char_, size_, s, n );
				swap_with(tmp);
			}
			
		}
		
		//------------------------------------------------------------------
		//
		// comparison
		//
		//------------------------------------------------------------------
		template <>
		int string<YCHAR>:: compare_blocks( const YCHAR *a, size_t p, const YCHAR *b, size_t q ) throw()
		{
			bool normal = true;
			if( p > q )
			{
				cswap( p, q );
				cswap( a, b );
				normal = false;
			}
			assert( p <= q );
			while( p > 0 )
			{
				const ptrdiff_t L = int(*a);
				const ptrdiff_t R = int(*b);
				
				if( L < R )
				{
					return normal ? -1 : 1;
				}
				else
				{
					
					if( L > R )
					{
						return normal ? 1 : -1;
					}
					else
					{
						++a;
						++b;
						--p;
						--q;
						continue;
					}
				}
			}
			
			//-- match to here
			return 0 == q ? 0 : ( normal ? -1 : 1 );
		}
		

		//----------------------------------------------------------------------
		//
		//
		//
		//----------------------------------------------------------------------
		template <>
		void string<YCHAR>:: trim(size_t n) throw()
		{
			if( n < size_ )
			{
				for( size_t i=n;i>0;--i) char_[--size_] = 0;
			}
			else 
			{
				clear();
			}
		}

        template <>
        void string<YCHAR>:: trim( bool (*bad)(YCHAR) ) throw()
        {
            if(bad)
            {
                while(size_>0 && bad(char_[size_-1]))
                    char_[--size_] = 0;
            }
        }
        
		template <>
		void string<YCHAR>:: skip(size_t n) throw()
		{
			if( n < size_ )
			{
				memmove( char_, char_+n, (size_-n) * sizeof(YCHAR) );
				size_ -= n;
				memset(  char_+size_, 0, (full_-size_) * sizeof(YCHAR) );
			}
			else 
			{
				clear();
			}
		}
        
        template <>
        void string<YCHAR>:: skip( bool (*bad)(YCHAR) ) throw()
        {
            if(bad)
            {
                size_t n=0;
                while( n<size_ && bad(char_[n])) ++n;
                skip(n);
            }
        }
        template <>
        void string<YCHAR>:: clean( bool (*bad)(YCHAR) ) throw()
		{
            trim(bad);
            skip(bad);
        }
        
        template <>
		void string<YCHAR>:: to_lower() throw()
        {
            static const int shift = int('a') - int('A');
            for( size_t i=0; i < size_; ++i )
            {
                int c = char_[i];
                if( c >= 'A' && c <= 'Z' )
                    char_[i] = char(c + shift);
            }
        }
        
        template <>
		void string<YCHAR>:: to_upper() throw()
        {
            static const int shift = int('A') - int('a');
            for( size_t i=0; i < size_; ++i )
            {
                int c = char_[i];
                if( c >= 'a' && c <= 'z' )
                    char_[i] = char(c + shift);
            }
        }

	}
	
}
