#include "yocto/string/basic.hpp"
#include "yocto/memory/pooled.hpp"
#Include "yocto/code/swap.hpp"

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
		string<YCHAR>:: string( const sting &other ) :
		full_( other.size_ + 1 ),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 ),
		size_( other.size_ )
		{
			memcpy( char_, other.char_, size_ * sizeof(YCHAR) ):
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
			string tmp( other );
			swap_with(other);
			return *this;
		}
		
		//------------------------------------------------------------------
		//
		// constructors
		//
		//------------------------------------------------------------------
		template <>
		string<YCHAR>:: string( size_t n, as_capacity_t & ) :
		full_(n+1),
		char_( kind<pooled>::acquire_as<YCHAR>(full_) ),
		maxi_( full_ - 1 ),
		size_( 0 )
		{
		}
		
		
		
	}
	
}