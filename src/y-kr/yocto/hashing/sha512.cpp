#include "yocto/hashing/sha512.hpp"

#include <cstring>

namespace yocto
{
	
	namespace hashing
	{
		/*
		 * 64-bit integer manipulation macros (big endian)
		 */
#define GET_UINT64_BE(n,b,i)                            \
{                                                       \
(n) = ( (uint64_t) (b)[(i)    ] << 56 )       \
| ( (uint64_t) (b)[(i) + 1] << 48 )       \
| ( (uint64_t) (b)[(i) + 2] << 40 )       \
| ( (uint64_t) (b)[(i) + 3] << 32 )       \
| ( (uint64_t) (b)[(i) + 4] << 24 )       \
| ( (uint64_t) (b)[(i) + 5] << 16 )       \
| ( (uint64_t) (b)[(i) + 6] <<  8 )       \
| ( (uint64_t) (b)[(i) + 7]       );      \
}
		
#define PUT_UINT64_BE(n,b,i)                            \
{                                                       \
(b)[(i)    ] = (unsigned char) ( (n) >> 56 );       \
(b)[(i) + 1] = (unsigned char) ( (n) >> 48 );       \
(b)[(i) + 2] = (unsigned char) ( (n) >> 40 );       \
(b)[(i) + 3] = (unsigned char) ( (n) >> 32 );       \
(b)[(i) + 4] = (unsigned char) ( (n) >> 24 );       \
(b)[(i) + 5] = (unsigned char) ( (n) >> 16 );       \
(b)[(i) + 6] = (unsigned char) ( (n) >>  8 );       \
(b)[(i) + 7] = (unsigned char) ( (n)       );       \
}
		
		/*
		 * Round constants
		 */
		static const uint64_t K[80] =
		{
			YOCTO_U64(0x428A2F98D728AE22),  YOCTO_U64(0x7137449123EF65CD),
			YOCTO_U64(0xB5C0FBCFEC4D3B2F),  YOCTO_U64(0xE9B5DBA58189DBBC),
			YOCTO_U64(0x3956C25BF348B538),  YOCTO_U64(0x59F111F1B605D019),
			YOCTO_U64(0x923F82A4AF194F9B),  YOCTO_U64(0xAB1C5ED5DA6D8118),
			YOCTO_U64(0xD807AA98A3030242),  YOCTO_U64(0x12835B0145706FBE),
			YOCTO_U64(0x243185BE4EE4B28C),  YOCTO_U64(0x550C7DC3D5FFB4E2),
			YOCTO_U64(0x72BE5D74F27B896F),  YOCTO_U64(0x80DEB1FE3B1696B1),
			YOCTO_U64(0x9BDC06A725C71235),  YOCTO_U64(0xC19BF174CF692694),
			YOCTO_U64(0xE49B69C19EF14AD2),  YOCTO_U64(0xEFBE4786384F25E3),
			YOCTO_U64(0x0FC19DC68B8CD5B5),  YOCTO_U64(0x240CA1CC77AC9C65),
			YOCTO_U64(0x2DE92C6F592B0275),  YOCTO_U64(0x4A7484AA6EA6E483),
			YOCTO_U64(0x5CB0A9DCBD41FBD4),  YOCTO_U64(0x76F988DA831153B5),
			YOCTO_U64(0x983E5152EE66DFAB),  YOCTO_U64(0xA831C66D2DB43210),
			YOCTO_U64(0xB00327C898FB213F),  YOCTO_U64(0xBF597FC7BEEF0EE4),
			YOCTO_U64(0xC6E00BF33DA88FC2),  YOCTO_U64(0xD5A79147930AA725),
			YOCTO_U64(0x06CA6351E003826F),  YOCTO_U64(0x142929670A0E6E70),
			YOCTO_U64(0x27B70A8546D22FFC),  YOCTO_U64(0x2E1B21385C26C926),
			YOCTO_U64(0x4D2C6DFC5AC42AED),  YOCTO_U64(0x53380D139D95B3DF),
			YOCTO_U64(0x650A73548BAF63DE),  YOCTO_U64(0x766A0ABB3C77B2A8),
			YOCTO_U64(0x81C2C92E47EDAEE6),  YOCTO_U64(0x92722C851482353B),
			YOCTO_U64(0xA2BFE8A14CF10364),  YOCTO_U64(0xA81A664BBC423001),
			YOCTO_U64(0xC24B8B70D0F89791),  YOCTO_U64(0xC76C51A30654BE30),
			YOCTO_U64(0xD192E819D6EF5218),  YOCTO_U64(0xD69906245565A910),
			YOCTO_U64(0xF40E35855771202A),  YOCTO_U64(0x106AA07032BBD1B8),
			YOCTO_U64(0x19A4C116B8D2D0C8),  YOCTO_U64(0x1E376C085141AB53),
			YOCTO_U64(0x2748774CDF8EEB99),  YOCTO_U64(0x34B0BCB5E19B48A8),
			YOCTO_U64(0x391C0CB3C5C95A63),  YOCTO_U64(0x4ED8AA4AE3418ACB),
			YOCTO_U64(0x5B9CCA4F7763E373),  YOCTO_U64(0x682E6FF3D6B2B8A3),
			YOCTO_U64(0x748F82EE5DEFB2FC),  YOCTO_U64(0x78A5636F43172F60),
			YOCTO_U64(0x84C87814A1F0AB72),  YOCTO_U64(0x8CC702081A6439EC),
			YOCTO_U64(0x90BEFFFA23631E28),  YOCTO_U64(0xA4506CEBDE82BDE9),
			YOCTO_U64(0xBEF9A3F7B2C67915),  YOCTO_U64(0xC67178F2E372532B),
			YOCTO_U64(0xCA273ECEEA26619C),  YOCTO_U64(0xD186B8C721C0C207),
			YOCTO_U64(0xEADA7DD6CDE0EB1E),  YOCTO_U64(0xF57D4F7FEE6ED178),
			YOCTO_U64(0x06F067AA72176FBA),  YOCTO_U64(0x0A637DC5A2C898A6),
			YOCTO_U64(0x113F9804BEF90DAE),  YOCTO_U64(0x1B710B35131C471B),
			YOCTO_U64(0x28DB77F523047D84),  YOCTO_U64(0x32CAAB7B40C72493),
			YOCTO_U64(0x3C9EBE0A15C9BEBC),  YOCTO_U64(0x431D67C49C100D4C),
			YOCTO_U64(0x4CC5D4BECB3E42B6),  YOCTO_U64(0x597F299CFC657E2A),
			YOCTO_U64(0x5FCB6FAB3AD6FAEC),  YOCTO_U64(0x6C44198C4A475817)
		};
		
		/*
		 * SHA-512 context setup
		 */
		static
		void sha512_starts( SHA512_CTX *ctx, const int is384 )
		{
			ctx->total[0] = 0;
			ctx->total[1] = 0;
			
			if( is384 == 0 )
			{
				/* SHA-512 */
				ctx->state[0] = YOCTO_U64(0x6A09E667F3BCC908);
				ctx->state[1] = YOCTO_U64(0xBB67AE8584CAA73B);
				ctx->state[2] = YOCTO_U64(0x3C6EF372FE94F82B);
				ctx->state[3] = YOCTO_U64(0xA54FF53A5F1D36F1);
				ctx->state[4] = YOCTO_U64(0x510E527FADE682D1);
				ctx->state[5] = YOCTO_U64(0x9B05688C2B3E6C1F);
				ctx->state[6] = YOCTO_U64(0x1F83D9ABFB41BD6B);
				ctx->state[7] = YOCTO_U64(0x5BE0CD19137E2179);
			}
			else
			{
				/* SHA-384 */
				ctx->state[0] = YOCTO_U64(0xCBBB9D5DC1059ED8);
				ctx->state[1] = YOCTO_U64(0x629A292A367CD507);
				ctx->state[2] = YOCTO_U64(0x9159015A3070DD17);
				ctx->state[3] = YOCTO_U64(0x152FECD8F70E5939);
				ctx->state[4] = YOCTO_U64(0x67332667FFC00B31);
				ctx->state[5] = YOCTO_U64(0x8EB44A8768581511);
				ctx->state[6] = YOCTO_U64(0xDB0C2E0D64F98FA7);
				ctx->state[7] = YOCTO_U64(0x47B5481DBEFA4FA4);
			}
			
			ctx->is384 = is384;
		}
		
		static
		void  sha512_set(  SHA512_CTX *ctx ) 
		{
			sha512_starts(ctx,0);
		}
		
		static
		void  sha384_set( SHA512_CTX *ctx ) 
		{
			sha512_starts(ctx,1);
		}
		
		
		static void sha512_process( SHA512_CTX *ctx, const uint8_t data[128] ) 
		{
			int i;
			uint64_t temp1, temp2, W[80];
			uint64_t A, B, C, D, E, F, G, H;
			
#define  SHR(x,n) (x >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (64 - n)))
			
#define S0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7))
#define S1(x) (ROTR(x,19) ^ ROTR(x,61) ^  SHR(x, 6))
			
#define S2(x) (ROTR(x,28) ^ ROTR(x,34) ^ ROTR(x,39))
#define S3(x) (ROTR(x,14) ^ ROTR(x,18) ^ ROTR(x,41))
			
#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))
			
#define P(a,b,c,d,e,f,g,h,x,K)                  \
{                                               \
temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
temp2 = S2(a) + F0(a,b,c);                  \
d += temp1; h = temp1 + temp2;              \
}
			
			for( i = 0; i < 16; i++ )
			{
				GET_UINT64_BE( W[i], data, i << 3 );
			}
			
			for( ; i < 80; i++ )
			{
				W[i] = S1(W[i -  2]) + W[i -  7] +
				S0(W[i - 15]) + W[i - 16];
			}
			
			A = ctx->state[0];
			B = ctx->state[1];
			C = ctx->state[2];
			D = ctx->state[3];
			E = ctx->state[4];
			F = ctx->state[5];
			G = ctx->state[6];
			H = ctx->state[7];
			i = 0;
			
			do
			{
				P( A, B, C, D, E, F, G, H, W[i], K[i] ); i++;
				P( H, A, B, C, D, E, F, G, W[i], K[i] ); i++;
				P( G, H, A, B, C, D, E, F, W[i], K[i] ); i++;
				P( F, G, H, A, B, C, D, E, W[i], K[i] ); i++;
				P( E, F, G, H, A, B, C, D, W[i], K[i] ); i++;
				P( D, E, F, G, H, A, B, C, W[i], K[i] ); i++;
				P( C, D, E, F, G, H, A, B, W[i], K[i] ); i++;
				P( B, C, D, E, F, G, H, A, W[i], K[i] ); i++;
			}
			while( i < 80 );
			
			ctx->state[0] += A;
			ctx->state[1] += B;
			ctx->state[2] += C;
			ctx->state[3] += D;
			ctx->state[4] += E;
			ctx->state[5] += F;
			ctx->state[6] += G;
			ctx->state[7] += H;
		}
		
		/*
		 * SHA-512 process buffer
		 */
		static
		void  sha512_run( SHA512_CTX *ctx, const void *buffer, const size_t buflen )
		{
			const uint8_t *input = (const uint8_t *)buffer;
			size_t    ilen  = buflen;
			size_t    fill;
			uint64_t left;
			
			if( ilen <= 0 )
				return;
			
			left = ctx->total[0] & 0x7F;
			fill = ( 128 - (size_t)left );
			
			ctx->total[0] += ilen;
			
			if( ctx->total[0] < (uint64_t) ilen )
				ctx->total[1]++;
			
			if( left && ilen >= fill )
			{
				memcpy( (void *) (ctx->buffer + left),
					   (void *) input, fill );
				sha512_process( ctx, ctx->buffer );
				input += fill;
				ilen  -= fill;
				left = 0;
			}
			
			while( ilen >= 128 )
			{
				sha512_process( ctx, input );
				input += 128;
				ilen  -= 128;
			}
			
			if( ilen > 0 )
			{
				memcpy( (void *) (ctx->buffer + left),
					   (void *) input, ilen );
			}
		}
		
		static const uint8_t sha512_padding[128] =
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		
		/*
		 * SHA-512 final digest
		 */
		static
		void sha512_compute( SHA512_CTX *ctx, uint8_t output[64] ) 
		{
			int last, padn;
			uint64_t high, low;
			uint8_t msglen[16];
			
			high = ( ctx->total[0] >> 61 )
			| ( ctx->total[1] <<  3 );
			low  = ( ctx->total[0] <<  3 );
			
			PUT_UINT64_BE( high, msglen, 0 );
			PUT_UINT64_BE( low,  msglen, 8 );
			
			last = (int)( ctx->total[0] & 0x7F );
			padn = ( last < 112 ) ? ( 112 - last ) : ( 240 - last );
			
			sha512_run( ctx, sha512_padding, padn );
			sha512_run( ctx, msglen, 16 );
			
			PUT_UINT64_BE( ctx->state[0], output,  0 );
			PUT_UINT64_BE( ctx->state[1], output,  8 );
			PUT_UINT64_BE( ctx->state[2], output, 16 );
			PUT_UINT64_BE( ctx->state[3], output, 24 );
			PUT_UINT64_BE( ctx->state[4], output, 32 );
			PUT_UINT64_BE( ctx->state[5], output, 40 );
			
			if( ctx->is384 == 0)
			{
				PUT_UINT64_BE( ctx->state[6], output, 48 );
				PUT_UINT64_BE( ctx->state[7], output, 56 );
			}
		}
		
		
		////////////////////////////////////////////////////////////////////////
		
		sha512::sha512() throw() : function(__length, __window), ctx()
		{
		}
		
		const char *sha512:: name() const throw()
		{
			return "SHA-512";
		}
		
		
		void sha512:: set() throw()
		{
			sha512_set( &ctx );
		}
		
		void sha512:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buflen>0&&NULL==buffer) );
			sha512_run( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void sha512::get(void *output, size_t outlen ) throw()
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[64];
			sha512_compute( &ctx, digest);
			fill( output, outlen, digest, __length );
		}
		
		sha512:: ~sha512() throw()
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}
		
		////////////////////////////////////////////////////////////////////////
		sha384::sha384() throw() : function(__length, __window), ctx()
		{
		}
		
		const char *sha384:: name() const throw()
		{
			return "SHA-384";
		}
		
		
		void sha384:: set() throw()
		{
			sha384_set( &ctx );
		}
		
		void sha384:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buflen>0&&NULL==buffer) );
			sha512_run( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void sha384::get(void *output, size_t outlen ) throw()
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[64];
			sha512_compute( &ctx, digest);
			fill( output, outlen, digest,__length );
		}
		
		sha384:: ~sha384() throw()
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}
		
		
	}
	
}

