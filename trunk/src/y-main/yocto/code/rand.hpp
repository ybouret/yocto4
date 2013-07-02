#ifndef YOCTO_CODE_RAND_INCLUDED
#define YOCTO_CODE_RAND_INCLUDED 1

#include "yocto/code/rand32.hpp"

namespace yocto
{
	
	extern urand32 &_rand;
	
    //! thread unsafe
    inline void alea_init() throw() { _rand.wseed(); }
    
	//! thread unsafe: float, double, uint32_t
	template <typename T> inline T    alea(void)    throw() { return _rand.get<T>(); }	

	//! thread unsafe: integer type
	template <typename T> inline T    alea_of(void) throw() { return _rand.full<T>(); }
	
	//! thread unsafe: 0..some_number
	inline size_t                     alea_leq( const size_t some_number )  throw() { return _rand.leq(some_number); }
	
    //! thread unsafe: 0..some_number-1
	inline size_t                     alea_lt( const size_t some_number  )  throw() { return _rand.lt(some_number); }
	
    
	//! thread unsafe
	template <typename T> inline void c_shuffle( T *base, size_t n ) throw() { _rand.shuffle(base,n);    }
}

#endif
