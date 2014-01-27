#ifndef YOCTO_CODE_ENDIAN_INCLUDED
#define YOCTO_CODE_ENDIAN_INCLUDED 1

#include "yocto/type/ints.hpp"

#if defined(YOCTO_BYTE_ORDER)
#	error "YOCTO_BYTE_ORDER should NOT be defined"
#endif

#define YOCTO_BIG_ENDIAN 0x1234
#define YOCTO_LIT_ENDIAN 0x3412

#if defined(__GNUC__)
#	if defined(__i386__) || defined(__x86_64__) || defined(__ia64__)
#		define YOCTO_BYTE_ORDER YOCTO_LIT_ENDIAN
#	endif
#	if defined(__ppc__)
#		define YOCTO_BYTE_ORDER YOCTO_BIG_ENDIAN
#	endif
#endif

#if defined(_MSC_VER)
#		define YOCTO_BYTE_ORDER YOCTO_LIT_ENDIAN
#endif

#if defined(__DMC__)
#		define YOCTO_BYTE_ORDER YOCTO_LIT_ENDIAN
#endif

#if !defined(YOCTO_BYTE_ORDER)
#	error "YOCTO_BYTE_ORDER could not be guessed!"
#endif


#define YOCTO_NOPE16(X) ((uint16_t)(X))

#define YOCTO_SWAP16(X)                \
(                                      \
( (((uint16_t)(X)) & 0xff00 ) >> 8 ) | \
( (((uint16_t)(X)) & 0x00ff ) << 8 )   \
)

#define YOCTO_NOPE32(X) ((uint32_t)(X))

#define YOCTO_SWAP32(X)                \
(                                      \
(((uint32_t)(X) & 0xff000000) >> 24) | \
(((uint32_t)(X) & 0x00ff0000) >> 8)  | \
(((uint32_t)(X) & 0x0000ff00) << 8)  | \
(((uint32_t)(X) & 0x000000ff) << 24)   \
)

#define YOCTO_NOPE64(X) ((uint64_t)(X))

#define YOCTO_MASK64(N) ( ( (uint64_t)(0xff) ) << (N) )

#define YOCTO_SWAP64(X)                       \
(                                             \
(((uint64_t)(X) & YOCTO_MASK64(56) ) >> 56) | \
(((uint64_t)(X) & YOCTO_MASK64(48) ) >> 40) | \
(((uint64_t)(X) & YOCTO_MASK64(40) ) >> 24) | \
(((uint64_t)(X) & YOCTO_MASK64(32) ) >>  8) | \
(((uint64_t)(X) & YOCTO_MASK64(24) ) <<  8) | \
(((uint64_t)(X) & YOCTO_MASK64(16) ) << 24) | \
(((uint64_t)(X) & YOCTO_MASK64( 8) ) << 40) | \
(((uint64_t)(X) & YOCTO_MASK64( 0) ) << 56)   \
)


#if YOCTO_BYTE_ORDER == YOCTO_BIG_ENDIAN

#define YOCTO_SWAP_BE16(X) YOCTO_NOPE16(X)
#define YOCTO_SWAP_LE16(X) YOCTO_SWAP16(X)

#define YOCTO_SWAP_BE32(X) YOCTO_NOPE32(X)
#define YOCTO_SWAP_LE32(X) YOCTO_SWAP32(X)

#define YOCTO_SWAP_BE64(X) YOCTO_NOPE64(X)
#define YOCTO_SWAP_LE64(X) YOCTO_SWAP64(X)

#else

#define YOCTO_SWAP_BE16(X) YOCTO_SWAP16(X)
#define YOCTO_SWAP_LE16(X) YOCTO_NOPE16(X)

#define YOCTO_SWAP_BE32(X) YOCTO_SWAP32(X)
#define YOCTO_SWAP_LE32(X) YOCTO_NOPE32(X)

#define YOCTO_SWAP_BE64(X) YOCTO_SWAP64(X)
#define YOCTO_SWAP_LE64(X) YOCTO_NOPE64(X)

#endif

namespace yocto {
	
    
    //__________________________________________________________________________
    //
    // big endian swapping
    //__________________________________________________________________________
    
	inline uint16_t swap_be16( uint16_t x ) throw() {
		return uint16_t(YOCTO_SWAP_BE16(x));
	}
	
	inline uint16_t swap_le16( uint16_t x ) throw() {
		return YOCTO_SWAP_LE16(x);
	}
	
	inline uint32_t swap_be32( uint32_t x ) throw() {
		return YOCTO_SWAP_BE32(x);
	}
	
	inline uint32_t swap_le32( uint32_t x ) throw() {
		return YOCTO_SWAP_LE32(x);
	}
	
	inline uint64_t swap_be64( uint64_t x ) throw() {
		return YOCTO_SWAP_BE64(x);
	}
	
	inline uint64_t swap_le64( uint64_t x ) throw() {
		return YOCTO_SWAP_LE64(x);
	}
	
	template <class T> T swap_be( const T &x ) throw();
	
	template <>
	inline uint8_t swap_be( const uint8_t &x ) throw() {
		return x;
	}
	
	
	template <>
	inline uint16_t swap_be( const uint16_t &x ) throw() {
		return swap_be16(x);
	}
	
	template <>
	inline uint32_t swap_be( const uint32_t &x ) throw() {
		return swap_be32(x);
	}
	
	template <>
	inline uint64_t swap_be( const uint64_t &x ) throw() {
		return swap_be64(x);
	}
	
	template <typename T>
	inline T swap_be_as( T x ) throw()
	{
		typedef typename unsigned_int<sizeof(T)>::type uint_T;
		union {
			T      item;
			uint_T bits;
		} alias = { x };
		alias.bits = swap_be<uint_T>(alias.bits);
		return alias.item;
	}
	
    template <typename T>
    inline void store_be( T x, void *addr ) throw()
    {
        assert(addr!=0);
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
		union {
			T      item;
			uint_T bits;
		} alias = { x };
		const uint_T y = swap_be<uint_T>(alias.bits);
        const char  *q = (const char *)&y;
        char        *p = (char       *)addr;
        for(size_t i=0;i<sizeof(T);++i)  { p[i] = q[i]; }
    }
    
    template <typename T>
    inline T query_be(const void *addr) throw()
    {
        assert(addr!=0);
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
		union {
			uint_T bits;
            T      item;
		} alias = { 0 };
        const char *q = (const char*)addr;
        char       *p = (char *) &alias.bits;
        for(size_t i=0;i<sizeof(T);++i) { p[i] = q[i]; }
        alias.bits = swap_be<uint_T>(alias.bits);
        return alias.item;
    }
    
    //__________________________________________________________________________
    //
    // little endian swapping
    //__________________________________________________________________________
    
	template <class T> T swap_le( const T &x ) throw();
	
	template <>
	inline uint8_t swap_le( const uint8_t &x ) throw() {
		return x;
	}
	
	template <>
	inline uint16_t swap_le( const uint16_t &x ) throw() {
		return swap_le16(x);
	}
	
	template <>
	inline uint32_t swap_le( const uint32_t &x ) throw() {
		return swap_le32(x);
	}
	
	template <>
	inline uint64_t swap_le( const uint64_t &x ) throw() {
		return swap_le64(x);
	}
	
	template <typename T>
	inline T swap_le_as( T x ) throw()
	{
		typedef typename unsigned_int<sizeof(T)>::type uint_T;
		union {
			T      item;
			uint_T bits;
		} alias = { x };
		alias.bits = swap_le<uint_T>(alias.bits);
		return alias.item;
	}
	
	
}


#endif
