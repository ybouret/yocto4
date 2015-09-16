#ifndef YOCTO_BMOVE_INCLUDED
#define YOCTO_BMOVE_INCLUDED 1

#include "yocto/type/ints.hpp"
#include "yocto/code/unroll.hpp"


namespace yocto
{
    namespace core
    {
        
        //______________________________________________________________________
        //
        // Default Slower Algorithm
        //______________________________________________________________________
#define Y_BMOVE_FUNC(I) tgt[I] = src[I]
        template <size_t N>
        inline void bmove( void *a, const void *b ) throw()
        {
            uint8_t       *tgt = (uint8_t *)a;
            const uint8_t *src = (const uint8_t *)b;
            YOCTO_LOOP_FUNC_(N, Y_BMOVE_FUNC, 0);
        }
        
        //______________________________________________________________________
        //
        // For Native Types
        //______________________________________________________________________
#define Y_BMOVE_IMPL(N) \
template <> inline void bmove<N>(void *a, const void *b) throw() \
{ \
typedef unsigned_int<N>::type word_t;\
*(word_t *)a = *(const word_t *)b;\
}
        Y_BMOVE_IMPL(1)
        Y_BMOVE_IMPL(2)
        Y_BMOVE_IMPL(4)
        Y_BMOVE_IMPL(8)
        
        
        //______________________________________________________________________
        //
        // For Extended Types
        //______________________________________________________________________
        
        template <> inline void bmove<12>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            
            uint32_t       *t32 = (uint32_t *)&tgt[1];
            const uint32_t *s32 = (const uint32_t *)&src[1];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<16>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
        }
        
        template <> inline void bmove<20>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            
            uint32_t       *t32 = (uint32_t *)&tgt[2];
            const uint32_t *s32 = (const uint32_t *)&src[2];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<24>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
        }
        
        template <> inline void bmove<28>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            
            uint32_t       *t32 = (uint32_t *)&tgt[3];
            const uint32_t *s32 = (const uint32_t *)&src[3];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<32>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
        }
        
        template <> inline void bmove<36>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            
            uint32_t       *t32 = (uint32_t *)&tgt[4];
            const uint32_t *s32 = (const uint32_t *)&src[4];
            t32[0] = s32[0];
        }
        
        
        template <> inline void bmove<40>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
        }
        
        template <> inline void bmove<44>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            
            uint32_t       *t32 = (uint32_t *)&tgt[5];
            const uint32_t *s32 = (const uint32_t *)&src[5];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<48>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            tgt[5] = src[5];
        }
        
        
        template <> inline void bmove<52>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            tgt[5] = src[5];
            
            uint32_t       *t32 = (uint32_t *)&tgt[6];
            const uint32_t *s32 = (const uint32_t *)&src[6];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<56>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            tgt[5] = src[5];
            tgt[6] = src[6];
        }
        
        template <> inline void bmove<60>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            tgt[5] = src[5];
            tgt[6] = src[6];
            
            uint32_t       *t32 = (uint32_t *)&tgt[7];
            const uint32_t *s32 = (const uint32_t *)&src[7];
            t32[0] = s32[0];
        }
        
        template <> inline void bmove<64>(void *a, const void *b) throw()
        {
            uint64_t       *tgt = (uint64_t *)a;
            const uint64_t *src = (const uint64_t *)b;
            tgt[0] = src[0];
            tgt[1] = src[1];
            tgt[2] = src[2];
            tgt[3] = src[3];
            tgt[4] = src[4];
            tgt[5] = src[5];
            tgt[6] = src[6];
            tgt[7] = src[7];
        }
        
        
    }
    
    template <typename T>
    inline void bmove( T &a, const T&b ) throw()
    {
        core::bmove<sizeof(T)>(&a, &b);
    }

}


#endif

