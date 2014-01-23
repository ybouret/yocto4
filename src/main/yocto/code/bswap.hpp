#ifndef YOCTO_BSWAP_INCLUDED
#define YOCTO_BSWAP_INCLUDED 1

#include "yocto/os.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace core
    {
#define Y_BSWAP_FUNC(TYPE,I) const TYPE tmp(L[I]); L[I] = R[I]; R[I] = tmp
#define Y_BSWAP8(I)          Y_BSWAP_FUNC(uint8_t,I)
        
        template <size_t N>
        inline void bswap( void *lhs, void *rhs ) throw()
        {
            uint8_t *L = (uint8_t *)lhs;
            uint8_t *R = (uint8_t *)rhs;
            YOCTO_LOOP_FUNC_(N, Y_BSWAP8, 0);
        }
        
#define YOCTO_BSWAP_IMPL(TYPE) \
const TYPE tmp = *(TYPE *)lhs; \
*(TYPE *)lhs   = *(TYPE *)rhs; \
*(TYPE *)rhs   = tmp
        
        template <>
        inline void bswap<1>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint8_t);
        }
        
        template <>
        inline void bswap<2>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint16_t);
        }
        
        template <>
        inline void bswap<4>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint32_t);
        }
        
        template <>
        inline void bswap<8>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint64_t);
        }
        
#define Y_BSWAP64(I) Y_BSWAP_FUNC(uint64_t,I)
        
        template <>
        inline void bswap<16>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
        }
        
        template <>
        inline void bswap<24>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
        }
        
        
        template <>
        inline void bswap<32>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
        }
        
        template <>
        inline void bswap<40>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
        }
        
        template <>
        inline void bswap<48>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
        }
        
        template <>
        inline void bswap<56>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
            { Y_BSWAP64(6); }
        }
        
        template <>
        inline void bswap<64>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
            { Y_BSWAP64(6); }
            { Y_BSWAP64(7); }
        }
 
    }
    
    template <typename T>
	inline void cswap( T &a, T &b ) throw()
	{
		const T tmp(a); a = b; b = tmp;
	}
	
	template <typename T>
	inline void cswap_const( const T &a, const T &b ) throw()
	{
		cswap<T>( (T&)a, (T&)b );
	}
    
    template <typename T>
    inline void bswap(T &a, T &b) throw()
    {
        core::bswap<sizeof(T)>( &a, &b );
    }
    
    
    template <typename T>
	inline void mreverse( T *base, size_t size ) throw()
	{
		if( size > 1 )
		{
			const size_t n = size/2;
			const size_t m = size-1;
			for( size_t i=0; i < n; ++i )
			{
                core::bswap<sizeof(T)>( &base[i], &base[m-i]);
			}
		}
	}
    
    inline void memswap( void *target, void *source, size_t length ) throw()
	{
		assert( !(NULL==target && length > 0 ) );
		assert( !(NULL==source && length > 0 ) );
		uint8_t *p = (uint8_t *)target;
		uint8_t *q = (uint8_t *)source;
		for( size_t i=length;i>0;--i,++p,++q) cswap( *p, *q );
	}
    
}


#endif

