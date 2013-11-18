#include "yocto/hashing/rmd128.hpp"
#include <cstring>
#include <iostream>

namespace yocto
{
    namespace hashing
    {
        
        namespace {
            /* collect four bytes into one word: */
#define BYTES_TO_DWORD(strptr)                    \
(((dword) *((strptr)+3) << 24) | \
((dword) *((strptr)+2) << 16) | \
((dword) *((strptr)+1) <<  8) | \
((dword) *(strptr)))
            
            /* ROL(x, n) cyclically rotates x over n bits to the left */
            /* x must be of an unsigned 32 bits type and 0 <= n < 32. */
#define ROL(x, n)        (((x) << (n)) | ((x) >> (32-(n))))
            
            /* the four basic functions F(), G() and H() */
#define F(x, y, z)        ((x) ^ (y) ^ (z))
#define G(x, y, z)        (((x) & (y)) | (~(x) & (z)))
#define H(x, y, z)        (((x) | ~(y)) ^ (z))
#define I(x, y, z)        (((x) & (z)) | ((y) & ~(z)))
            
            /* the eight basic operations FF() through III() */
#define FF(a, b, c, d, x, s)        {\
(a) += F((b), (c), (d)) + (x);\
(a) = ROL((a), (s));\
}
#define GG(a, b, c, d, x, s)        {\
(a) += G((b), (c), (d)) + (x) + 0x5a827999UL;\
(a) = ROL((a), (s));\
}
#define HH(a, b, c, d, x, s)        {\
(a) += H((b), (c), (d)) + (x) + 0x6ed9eba1UL;\
(a) = ROL((a), (s));\
}
#define II(a, b, c, d, x, s)        {\
(a) += I((b), (c), (d)) + (x) + 0x8f1bbcdcUL;\
(a) = ROL((a), (s));\
}
#define FFF(a, b, c, d, x, s)        {\
(a) += F((b), (c), (d)) + (x);\
(a) = ROL((a), (s));\
}
#define GGG(a, b, c, d, x, s)        {\
(a) += G((b), (c), (d)) + (x) + 0x6d703ef3UL;\
(a) = ROL((a), (s));\
}
#define HHH(a, b, c, d, x, s)        {\
(a) += H((b), (c), (d)) + (x) + 0x5c4dd124UL;\
(a) = ROL((a), (s));\
}
#define III(a, b, c, d, x, s)        {\
(a) += I((b), (c), (d)) + (x) + 0x50a28be6UL;\
(a) = ROL((a), (s));\
}
            
            static inline
            void MDinit(uint32_t *MDbuf)
            {
                MDbuf[0] = 0x67452301UL;
                MDbuf[1] = 0xefcdab89UL;
                MDbuf[2] = 0x98badcfeUL;
                MDbuf[3] = 0x10325476UL;
            }
            
            static inline
            void compress(uint32_t *MDbuf, const uint32_t *X)
            {
                uint32_t  aa = MDbuf[0],  bb = MDbuf[1],  cc = MDbuf[2],  dd = MDbuf[3];
                uint32_t aaa = MDbuf[0], bbb = MDbuf[1], ccc = MDbuf[2], ddd = MDbuf[3];
                
                /* round 1 */
                FF(aa, bb, cc, dd, X[ 0], 11);
                FF(dd, aa, bb, cc, X[ 1], 14);
                FF(cc, dd, aa, bb, X[ 2], 15);
                FF(bb, cc, dd, aa, X[ 3], 12);
                FF(aa, bb, cc, dd, X[ 4],  5);
                FF(dd, aa, bb, cc, X[ 5],  8);
                FF(cc, dd, aa, bb, X[ 6],  7);
                FF(bb, cc, dd, aa, X[ 7],  9);
                FF(aa, bb, cc, dd, X[ 8], 11);
                FF(dd, aa, bb, cc, X[ 9], 13);
                FF(cc, dd, aa, bb, X[10], 14);
                FF(bb, cc, dd, aa, X[11], 15);
                FF(aa, bb, cc, dd, X[12],  6);
                FF(dd, aa, bb, cc, X[13],  7);
                FF(cc, dd, aa, bb, X[14],  9);
                FF(bb, cc, dd, aa, X[15],  8);
                
                /* round 2 */
                GG(aa, bb, cc, dd, X[ 7],  7);
                GG(dd, aa, bb, cc, X[ 4],  6);
                GG(cc, dd, aa, bb, X[13],  8);
                GG(bb, cc, dd, aa, X[ 1], 13);
                GG(aa, bb, cc, dd, X[10], 11);
                GG(dd, aa, bb, cc, X[ 6],  9);
                GG(cc, dd, aa, bb, X[15],  7);
                GG(bb, cc, dd, aa, X[ 3], 15);
                GG(aa, bb, cc, dd, X[12],  7);
                GG(dd, aa, bb, cc, X[ 0], 12);
                GG(cc, dd, aa, bb, X[ 9], 15);
                GG(bb, cc, dd, aa, X[ 5],  9);
                GG(aa, bb, cc, dd, X[ 2], 11);
                GG(dd, aa, bb, cc, X[14],  7);
                GG(cc, dd, aa, bb, X[11], 13);
                GG(bb, cc, dd, aa, X[ 8], 12);
                
                /* round 3 */
                HH(aa, bb, cc, dd, X[ 3], 11);
                HH(dd, aa, bb, cc, X[10], 13);
                HH(cc, dd, aa, bb, X[14],  6);
                HH(bb, cc, dd, aa, X[ 4],  7);
                HH(aa, bb, cc, dd, X[ 9], 14);
                HH(dd, aa, bb, cc, X[15],  9);
                HH(cc, dd, aa, bb, X[ 8], 13);
                HH(bb, cc, dd, aa, X[ 1], 15);
                HH(aa, bb, cc, dd, X[ 2], 14);
                HH(dd, aa, bb, cc, X[ 7],  8);
                HH(cc, dd, aa, bb, X[ 0], 13);
                HH(bb, cc, dd, aa, X[ 6],  6);
                HH(aa, bb, cc, dd, X[13],  5);
                HH(dd, aa, bb, cc, X[11], 12);
                HH(cc, dd, aa, bb, X[ 5],  7);
                HH(bb, cc, dd, aa, X[12],  5);
                
                /* round 4 */
                II(aa, bb, cc, dd, X[ 1], 11);
                II(dd, aa, bb, cc, X[ 9], 12);
                II(cc, dd, aa, bb, X[11], 14);
                II(bb, cc, dd, aa, X[10], 15);
                II(aa, bb, cc, dd, X[ 0], 14);
                II(dd, aa, bb, cc, X[ 8], 15);
                II(cc, dd, aa, bb, X[12],  9);
                II(bb, cc, dd, aa, X[ 4],  8);
                II(aa, bb, cc, dd, X[13],  9);
                II(dd, aa, bb, cc, X[ 3], 14);
                II(cc, dd, aa, bb, X[ 7],  5);
                II(bb, cc, dd, aa, X[15],  6);
                II(aa, bb, cc, dd, X[14],  8);
                II(dd, aa, bb, cc, X[ 5],  6);
                II(cc, dd, aa, bb, X[ 6],  5);
                II(bb, cc, dd, aa, X[ 2], 12);
                
                /* parallel round 1 */
                III(aaa, bbb, ccc, ddd, X[ 5],  8);
                III(ddd, aaa, bbb, ccc, X[14],  9);
                III(ccc, ddd, aaa, bbb, X[ 7],  9);
                III(bbb, ccc, ddd, aaa, X[ 0], 11);
                III(aaa, bbb, ccc, ddd, X[ 9], 13);
                III(ddd, aaa, bbb, ccc, X[ 2], 15);
                III(ccc, ddd, aaa, bbb, X[11], 15);
                III(bbb, ccc, ddd, aaa, X[ 4],  5);
                III(aaa, bbb, ccc, ddd, X[13],  7);
                III(ddd, aaa, bbb, ccc, X[ 6],  7);
                III(ccc, ddd, aaa, bbb, X[15],  8);
                III(bbb, ccc, ddd, aaa, X[ 8], 11);
                III(aaa, bbb, ccc, ddd, X[ 1], 14);
                III(ddd, aaa, bbb, ccc, X[10], 14);
                III(ccc, ddd, aaa, bbb, X[ 3], 12);
                III(bbb, ccc, ddd, aaa, X[12],  6);
                
                /* parallel round 2 */
                HHH(aaa, bbb, ccc, ddd, X[ 6],  9);
                HHH(ddd, aaa, bbb, ccc, X[11], 13);
                HHH(ccc, ddd, aaa, bbb, X[ 3], 15);
                HHH(bbb, ccc, ddd, aaa, X[ 7],  7);
                HHH(aaa, bbb, ccc, ddd, X[ 0], 12);
                HHH(ddd, aaa, bbb, ccc, X[13],  8);
                HHH(ccc, ddd, aaa, bbb, X[ 5],  9);
                HHH(bbb, ccc, ddd, aaa, X[10], 11);
                HHH(aaa, bbb, ccc, ddd, X[14],  7);
                HHH(ddd, aaa, bbb, ccc, X[15],  7);
                HHH(ccc, ddd, aaa, bbb, X[ 8], 12);
                HHH(bbb, ccc, ddd, aaa, X[12],  7);
                HHH(aaa, bbb, ccc, ddd, X[ 4],  6);
                HHH(ddd, aaa, bbb, ccc, X[ 9], 15);
                HHH(ccc, ddd, aaa, bbb, X[ 1], 13);
                HHH(bbb, ccc, ddd, aaa, X[ 2], 11);
                
                /* parallel round 3 */
                GGG(aaa, bbb, ccc, ddd, X[15],  9);
                GGG(ddd, aaa, bbb, ccc, X[ 5],  7);
                GGG(ccc, ddd, aaa, bbb, X[ 1], 15);
                GGG(bbb, ccc, ddd, aaa, X[ 3], 11);
                GGG(aaa, bbb, ccc, ddd, X[ 7],  8);
                GGG(ddd, aaa, bbb, ccc, X[14],  6);
                GGG(ccc, ddd, aaa, bbb, X[ 6],  6);
                GGG(bbb, ccc, ddd, aaa, X[ 9], 14);
                GGG(aaa, bbb, ccc, ddd, X[11], 12);
                GGG(ddd, aaa, bbb, ccc, X[ 8], 13);
                GGG(ccc, ddd, aaa, bbb, X[12],  5);
                GGG(bbb, ccc, ddd, aaa, X[ 2], 14);
                GGG(aaa, bbb, ccc, ddd, X[10], 13);
                GGG(ddd, aaa, bbb, ccc, X[ 0], 13);
                GGG(ccc, ddd, aaa, bbb, X[ 4],  7);
                GGG(bbb, ccc, ddd, aaa, X[13],  5);
                
                /* parallel round 4 */
                FFF(aaa, bbb, ccc, ddd, X[ 8], 15);
                FFF(ddd, aaa, bbb, ccc, X[ 6],  5);
                FFF(ccc, ddd, aaa, bbb, X[ 4],  8);
                FFF(bbb, ccc, ddd, aaa, X[ 1], 11);
                FFF(aaa, bbb, ccc, ddd, X[ 3], 14);
                FFF(ddd, aaa, bbb, ccc, X[11], 14);
                FFF(ccc, ddd, aaa, bbb, X[15],  6);
                FFF(bbb, ccc, ddd, aaa, X[ 0], 14);
                FFF(aaa, bbb, ccc, ddd, X[ 5],  6);
                FFF(ddd, aaa, bbb, ccc, X[12],  9);
                FFF(ccc, ddd, aaa, bbb, X[ 2], 12);
                FFF(bbb, ccc, ddd, aaa, X[13],  9);
                FFF(aaa, bbb, ccc, ddd, X[ 9], 12);
                FFF(ddd, aaa, bbb, ccc, X[ 7],  5);
                FFF(ccc, ddd, aaa, bbb, X[10], 15);
                FFF(bbb, ccc, ddd, aaa, X[14],  8);
                
                /* combine results */
                ddd += cc + MDbuf[1];               /* final result for MDbuf[0] */
                MDbuf[1] = MDbuf[2] + dd + aaa;
                MDbuf[2] = MDbuf[3] + aa + bbb;
                MDbuf[3] = MDbuf[0] + bb + ccc;
                MDbuf[0] = ddd;
                
                return;
            }
            
            static inline
            void MDfinish(uint32_t *MDbuf,
                          const uint8_t *strptr,
                          uint32_t lswlen,
                          uint32_t mswlen)
            {
                unsigned int i;                                 /* counter       */
                uint32_t     X[16];                             /* message words */
                
                memset(X, 0, 16*sizeof(uint32_t));
                
                /* put bytes from strptr into X */
                for (i=0; i<(lswlen&63); i++) {
                    /* byte i goes into word X[i div 4] at pos.  8*(i mod 4)  */
                    X[i>>2] ^= (uint32_t) *strptr++ << (8 * (i&3));
                }
                
                /* append the bit m_n == 1 */
                X[(lswlen>>2)&15] ^= (uint32_t)1 << (8*(lswlen&3) + 7);
                
                if ((lswlen & 63) > 55) {
                    /* length goes to next block */
                    compress(MDbuf, X);
                    memset(X, 0, 16*sizeof(uint32_t));
                }
                
                /* append length in bits*/
                X[14] = lswlen << 3;
                X[15] = (lswlen >> 29) | (mswlen << 3);
                compress(MDbuf, X);
                
                return;
            }
            
        }
        
#if defined(_MSC_VER)
		// init MDbuf
#pragma warning ( disable : 4351 )
#endif
        rmd128 :: rmd128()  throw() :
        function(__length,__window),
        RMD(),
        MDbuf()
        {
        }
        
        rmd128:: ~rmd128() throw() {}
        
        const char * rmd128:: name()   const throw()
        {
            return "RMD-128";
        }
        
        
        
        void rmd128:: set() throw()
        {
            RMD.reset();
            MDinit(MDbuf);
        }
        
        void rmd128:: run(const void *buf, size_t len) throw()
        {
            const uint8_t *p = (const uint8_t *)buf;
            for( size_t i=len;i>0;--i,++p)
            {
                if( RMD.store( *p ) )
                    compress(MDbuf, RMD.block());
            }
        }
        
        void rmd128::get(void *output, size_t outlen) throw()
        {
            MDfinish(MDbuf, RMD.flush(), RMD.lswlen(), RMD.mswlen() );
            uint8_t hashcode[RMDsize/8];
            for(size_t i=0; i<RMDsize/8; i+=4)
            {
                hashcode[i]   = uint8_t(MDbuf[i>>2]);        /* implicit cast to byte  */
                hashcode[i+1] = uint8_t(MDbuf[i>>2] >>  8);  /*  extracts the 8 least  */
                hashcode[i+2] = uint8_t(MDbuf[i>>2] >> 16);  /*  significant bits.     */
                hashcode[i+3] = uint8_t(MDbuf[i>>2] >> 24);
            }
            fill(output, outlen, hashcode, sizeof(hashcode));
        }
        
        
    }
    
}
