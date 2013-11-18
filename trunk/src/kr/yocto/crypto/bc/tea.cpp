#include "yocto/crypto/bc/tea.hpp"

namespace yocto {
    
	namespace crypto {
        
		static const uint32_t tea_mix[64] =
        {
            0x00000000, /* n=00 (undefined)*/
            0x00000000, /* n=01 (undefined)*/
            0x00000020, /* n=02*/
            0x00000017, /* n=03*/
            0x00000013, /* n=04*/
            0x00000010, /* n=05*/
            0x0000000e, /* n=06*/
            0x0000000d, /* n=07*/
            0x0000000c, /* n=08*/
            0x0000000b, /* n=09*/
            0x0000000b, /* n=10*/
            0x0000000a, /* n=11*/
            0x0000000a, /* n=12*/
            0x0000000a, /* n=13*/
            0x00000009, /* n=14*/
            0x00000009, /* n=15*/
            0x00000009, /* n=16*/
            0x00000009, /* n=17*/
            0x00000008, /* n=18*/
            0x00000008, /* n=19*/
            0x00000008, /* n=20*/
            0x00000008, /* n=21*/
            0x00000008, /* n=22*/
            0x00000008, /* n=23*/
            0x00000008, /* n=24*/
            0x00000008, /* n=25*/
            0x00000008, /* n=26*/
            0x00000007, /* n=27*/
            0x00000007, /* n=28*/
            0x00000007, /* n=29*/
            0x00000007, /* n=30*/
            0x00000007, /* n=31*/
            0x00000007, /* n=32*/
            0x00000007, /* n=33*/
            0x00000007, /* n=34*/
            0x00000007, /* n=35*/
            0x00000007, /* n=36*/
            0x00000007, /* n=37*/
            0x00000007, /* n=38*/
            0x00000007, /* n=39*/
            0x00000007, /* n=40*/
            0x00000007, /* n=41*/
            0x00000007, /* n=42*/
            0x00000007, /* n=43*/
            0x00000007, /* n=44*/
            0x00000007, /* n=45*/
            0x00000007, /* n=46*/
            0x00000007, /* n=47*/
            0x00000007, /* n=48*/
            0x00000007, /* n=49*/
            0x00000007, /* n=50*/
            0x00000007, /* n=51*/
            0x00000007, /* n=52*/
            0x00000006  /* n>52*/
        };
        
        
		static const uint32_t tea_delta = 0x9E3779B9;
        
		void tea_encrypt(uint32_t *v, const size_t n, const uint32_t *k) throw() {
			assert(n>=2);
			assert(k!=NULL);
			assert(v!=NULL);
            
			{
				uint32_t z   = v[n-1];// y = v[0];
				uint32_t q   = n > 52 ? 0x00000006 : tea_mix[n];
				uint32_t sum = 0;
                
                
				while( q-- > 0 ) /* 6 + 52/n operations gives between 6 & 32 mixes on each word */
				{
					const size_t n1 = n-1;
					uint32_t     mx,e;
					register size_t p;
					sum += tea_delta;
					e = (sum>>2) & 3;
					for( p = 0; p < n1; p++ )
					{
						const uint32_t y  = v[p+1];
						mx = ( (z>>5) ^ (y<<2) ) + (( (y>>3) ^ (z<<4) ) ^ (sum^y)) + (k[ (p&3) ^ e] ^ z);
						z  = (v[p] += mx);
					}
                    {
                        const uint32_t y  = v[0];
                        mx = (z>>5 ^ y<<2) + ((y>>3 ^ z<<4) ^ (sum^y)) + (k[ (p&3) ^ e] ^ z);
                    }
					z = v[n-1] += mx;
				}
                
			}
            
		}
        
		void tea_decrypt(uint32_t *v, const size_t n, const uint32_t *k) throw() {
			assert(n>=2);
			assert(k!=NULL);
			assert(v!=NULL);
			{
                uint32_t y = v[0];
				uint32_t q = n > 52 ? 0x00000006 : tea_mix[n];
				uint32_t sum = q*tea_delta;
                
				while (sum != 0){
					const size_t n1 = n-1;
					uint32_t  mx;
					register size_t p;
					uint32_t e = (sum>>2) & 3;
					for( p = n1; p > 0; p-- )
					{
						const uint32_t z = v[p-1];
						mx = ((z>>5) ^ (y<<2)) + (((y>>3) ^ (z<<4)) ^ (sum^y)) + (k[(p&3) ^ e] ^ z);
						y = v[p] -= mx;
					}
                    {
                        const uint32_t z  = v[n1];
                        mx = ( (z>>5) ^ (y<<2) ) + (( (y>>3) ^ (z<<4)) ^ (sum^y)) + (k[e] ^ z);
                    }
					y  = (v[0] -= mx);
					sum -= tea_delta;
				}
			}
		}
        
	}
    
}
