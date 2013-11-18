#include "yocto/hashing/digest.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>
#include <iostream>

namespace yocto {


#define YOCTO_DIGEST_ROUND(N) YOCTO_ROUND(3,N)

	static uint8_t *acquire_digest( size_t n )
	{
		assert(n>0);
		return  static_cast<uint8_t *>( object::operator new(  YOCTO_DIGEST_ROUND(n) ) );
	}

	static void release_digest( void *p, size_t n ) throw()
	{
		assert(p!=NULL);
		assert(n>0);
		memset( p, 0, n );
		object::operator delete(p, YOCTO_DIGEST_ROUND(n) );
	}

	digest:: digest( size_t n, uint8_t fill ) :
			length_( max_of<size_t>(n,1)     ),
			buffer_( acquire_digest(length_) )
	{
		memset(buffer_,fill,length_);
	}


	size_t digest:: length() const throw()
	{
		return length_;
	}

	const void *digest:: get_address() const throw()
	{
		return (void *)buffer_;
	}

	digest:: ~digest() throw()
	{
		release_digest(buffer_,length_);
	}

	digest:: digest( const digest &other ) :
		length_( other.length_ ),
		buffer_(  acquire_digest(length_) )
	{
		memcpy( buffer_, other.buffer_, length_);
	}

	digest:: digest( const ro_buffer &buff ) :
		length_( buff.length() ),
		buffer_( acquire_digest(length_) )
	{
		memcpy( buffer_, buff.ro(), length_ );
	}


	digest & digest:: operator=( const digest &other ) throw()
	{
		this->assign( other );
		return *this;
	}

	digest & digest:: operator=( const ro_buffer &buff ) throw()
	{
		this->assign( buff );
		return *this;
	}

	void digest:: assign( const ro_buffer &buff ) throw()
	{
		const size_t len = buff.length();
		if( len >= length_ )
		{
			memmove( buffer_, buff.ro(), length_ );
		}
		else
		{
			// len < length_
			memmove( buffer_, buff.ro(), len );
			memset( buffer_ + len, 0, length_ - len );
		}
	}

	void digest:: get_lsb( void *p, size_t n ) const throw()
	{
		assert(!(p==NULL&&n>0));

		if( n >= length_ )
		{
			const size_t delta = n - length_;
			uint8_t *q = (uint8_t *)p;
			memset( q, 0, delta );
			memcpy( q+delta, buffer_, length_ );
		}
		else
		{
			// n < length
			memcpy( p, &buffer_[length_-n], n );
		}
	}

	void digest:: set_lsb( const void *p, size_t n ) throw()
	{
		assert(!(p==NULL&&n>0));
		if( n < length_ )
		{
				const size_t delta = length_ - n;
				memset( buffer_, 0, delta );
				memcpy( buffer_ + delta, p, n );
		}
		else
		{
			// n >= length
			const size_t delta = n - length_;
			memcpy( buffer_, (const uint8_t *)p + delta, length_ );
		}
	}



	uint8_t &digest:: operator[]( size_t index ) throw()
	{
		assert( index < length_ );
		return buffer_[index];
	}

	const uint8_t &digest:: operator[]( size_t index ) const throw()
	{
		assert( index < length_ );
		return buffer_[index];
	}

	std::ostream & operator<<( std::ostream &os, const digest &d ) {

		const uint8_t *p = d.buffer_;
		for( size_t j=d.length_;j>0;--j,++p) {
			const int B  = *p;
			const int hi = (B>>4);
			const int lo = (B&0xf);

			os << hexa_char[hi];
			os << hexa_char[lo];
		}
		return os;

	}
	
	bool operator==( const digest &lhs, const digest &rhs ) throw()
	{
		const uint8_t *pL = lhs.buffer_;
		const uint8_t *pR = rhs.buffer_;
		size_t         nL = lhs.length_;
		size_t         nR = rhs.length_;
		if( nR < nL )
		{
			cswap( nL, nR );
			cswap( pL, pR );
		}
		assert( nL <= nR );
		for( size_t i=0; i < nL; ++i )
		{
			if( pL[i] != pR[i] )
				return false;
		}
		for( size_t i=nL; i < nR; ++i )
		{
			if( pR[i] != 0 )
				return false;
		}
		return true;
	}
	
	bool operator != ( const digest &lhs, const digest &rhs ) throw()
	{
		return ! ( lhs == rhs );
	}
	



}
