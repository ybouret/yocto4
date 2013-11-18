#include "yocto/hashing/digest.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>

namespace yocto {

#if defined(__INTEL_COMPILER)
#pragma warning ( disable : 2259 )
#endif

	void digest:: _set( const uint8_t x ) throw() {
		memset( buffer_, x, length_ );
	}

	void digest:: _set( const digest &other ) throw() {
		assert( this != &other );
		assert( length_ == other.length_ );
		memcpy( buffer_, other.buffer_, length_ );
	}

	void digest::_xor( const digest &other ) throw() {
		assert( length_ == other.length_ );
		uint8_t       *tgt = buffer_;
		const uint8_t *src = other.buffer_;
		for( size_t i=length_;i>0;--i) {
			*(tgt++) ^= *(src++);
		}
	}

	void digest::_xor( const digest &lhs, const digest &rhs ) throw() {
		assert( length_ == lhs.length_ );
		assert( length_ == rhs.length_ );
		uint8_t       *t = buffer_;
		const uint8_t *l = lhs.buffer_;
		const uint8_t *r = rhs.buffer_;
		for( size_t i=length_;i>0;--i) {
			*(t++) = *(l++) ^ *(r++);
		}
	}

	void digest:: _swp( digest &other ) throw() {
		assert( length_ == other.length_ );
		cswap( buffer_, other.buffer_ );
	}


	void digest:: _inc(const uint8_t x) throw() {

		int sum = x;
		for( size_t i=0; i < length_; ++i ) {
			sum += buffer_[i];
			buffer_[i] = ( sum & 0xff );
			sum >>= 8;
		}

	}

}
