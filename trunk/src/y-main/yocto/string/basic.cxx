#include "yocto/string/basic.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/code/swap.hpp"

#include <cstring>

namespace yocto
{
	using namespace memory;

	namespace basic
	{
#define YCHECK_STRING(S) \
	assert( (S)->full_ > 0 ); \
	assert( (S)->char_ != NULL ); \
	assert( (S)->maxi_ == (S)->full_ - 1 );\
	assert( (S)->size_ <= (S)->maxi_ )



		template <>
		string<YCHAR>:: ~string() throw()
		{
			YCHECK_STRING(this);
			kind<pooled>::release_as<YCHAR>( char_, full_ );
			maxi_ = 0;
			size_ = 0;
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
		}

		template <>
		string<YCHAR> & string<YCHAR>:: operator=( const string &other )
		{
			YCHECK_STRING(this);
			YCHECK_STRING(&other);
			string tmp( other );
			swap_with(tmp);
			return *this;
		}

		//------------------------------------------------------------------
		//
		// constructors
		//
		//------------------------------------------------------------------
		template <>
		string<YCHAR>:: string( size_t n, as_capacity_t & ) :
		size_(0),
			full_(n+1),
			char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
			maxi_( full_ - 1 )
		{
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
		string<YCHAR>:: string( const YCHAR *a, size_t a_len, const YCHAR *b, size_t b_len ) :
		size_( a_len + b_len ),
			full_( size_ + 1     ),
			char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
			maxi_( full_ - 1 )
		{
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
		void string<YCHAR>:: assign( const YCHAR *s )
		{
			const size_t len = length_of(s);
			if( len <= maxi_ )
			{
				memcpy( char_,     s, (size_=len) * sizeof(YCHAR) );
				memset( char_+len, 0, (full_-size_)*sizeof(YCHAR) );
			}
			else
			{
				string tmp(s);
				swap_with(tmp);
			}
			YCHECK_STRING(this);
		}

		template <>
		void string<YCHAR>:: assign( const string &s )
		{
			const size_t len = s.size_;
			if( len <= maxi_ )
			{
				memcpy( char_,     s.char_, (size_=len) * sizeof(YCHAR) );
				memset( char_+len, 0, (full_-size_)*sizeof(YCHAR) );
			}
			else
			{
				string tmp(s);
				swap_with(tmp);
			}
			YCHECK_STRING(this);
		}
	}

}