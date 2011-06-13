#include "yocto/hashing/md2.hpp"
#include <cstring>

namespace yocto
{
	namespace hashing
	{
		
		namespace RFC1319
		{
			
			/* Permutation of 0..255 constructed from the digits of pi. It gives a
			 "random" nonlinear byte substitution operation.
			 */
			static uint8_t PI_SUBST[256] = 
			{
				41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
				19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
				76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
				138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
				245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
				148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
				39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
				181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
				150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
				112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
				96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
				85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
				234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
				129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
				8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
				203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
				166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
				31, 26, 219, 153, 141, 51, 159, 17, 131, 20
			};
			
			static uint8_t *PADDING[] =
			{
				(uint8_t *)"",
				(uint8_t *)"\001",
				(uint8_t *)"\002\002",
				(uint8_t *)"\003\003\003",
				(uint8_t *)"\004\004\004\004",
				(uint8_t *)"\005\005\005\005\005",
				(uint8_t *)"\006\006\006\006\006\006",
				(uint8_t *)"\007\007\007\007\007\007\007",
				(uint8_t *)"\010\010\010\010\010\010\010\010",
				(uint8_t *)"\011\011\011\011\011\011\011\011\011",
				(uint8_t *)"\012\012\012\012\012\012\012\012\012\012",
				(uint8_t *)"\013\013\013\013\013\013\013\013\013\013\013",
				(uint8_t *)"\014\014\014\014\014\014\014\014\014\014\014\014",
				(uint8_t *)
				"\015\015\015\015\015\015\015\015\015\015\015\015\015",
				(uint8_t *)
				"\016\016\016\016\016\016\016\016\016\016\016\016\016\016",
				(uint8_t *)
				"\017\017\017\017\017\017\017\017\017\017\017\017\017\017\017",
				(uint8_t *)
				"\020\020\020\020\020\020\020\020\020\020\020\020\020\020\020\020"
			};
			
			/* MD2 basic transformation. Transforms state and updates checksum
			 based on block.
			 */
			static void MD2Transform(
									 uint8_t       state[16],
									 uint8_t       checksum[16],
									 const uint8_t block[16]
									 )
			{
				unsigned int i, j, t;
				unsigned char x[48];
				
				/* Form encryption block from state, block, state ^ block.
				 */
				memcpy(x,      state, 16);
				memcpy(x+16,   block, 16);
				for (i = 0; i < 16; i++)
					x[i+32] = (unsigned char)(state[i] ^ block[i]);
				
				/* Encrypt block (18 rounds).
				 */
#if defined(__INTEL_COMPILER)
#pragma warning ( disable : 2259 )
#endif
				t = 0;
				for (i = 0; i < 18; i++) {
					for (j = 0; j < 48; j++)
					{
						t = (unsigned char)(x[j] ^= PI_SUBST[t]);
					}
					t = (t + i) & 0xff;
				}
				
				/* Save new state */
				memcpy( state, x, 16);
				
				/* Update checksum.
				 */
				t = checksum[15];
				for (i = 0; i < 16; i++)
					t = checksum[i] ^= PI_SUBST[block[i] ^ t];
				
				/* Zeroize sensitive information.
				 */
				memset(x, 0, sizeof(x));
			}
			
			
			/* MD2 initialization. Begins an MD2 operation, writing a new context.
			 */
			static void MD2Init(MD2_CTX *context )                                 /* context */
			{
				assert(context);
				memset( context, 0, sizeof(MD2_CTX) );
			}
			
			/* MD2 block update operation. Continues an MD2 message-digest
			 operation, processing another message block, and updating the
			 context.
			 */
			static void MD2Update(MD2_CTX             *context,                     /* context */
								  const void          *inputBuf,                    /* input block */
								  const size_t         inputLen                     /* length of input block */
								  )
			{
				
				const uint8_t *input = (const uint8_t *)inputBuf;
				/* Update number of bytes mod 16 */
				size_t         index = context->count;
				const size_t   partLen = 16 - index;
				size_t         i=0;
				context->count = (index + inputLen) & 0xf;
				
				
				/* Transform as many times as possible.
				 */
				
				if (inputLen >= partLen)
				{
					
					memcpy( &context->buffer[index], input, partLen);
					MD2Transform(context->state, context->checksum, context->buffer);
					
					for ( i = partLen; i + 15 < inputLen; i += 16)
						MD2Transform(context->state, context->checksum, &input[i]);
					
					index = 0;
				}
				
				
				/* Buffer remaining input */
				memcpy(&context->buffer[index], &input[i],inputLen-i);
			}
			
			/* MD2 finalization. Ends an MD2 message-digest operation, writing the
			 message digest and zeroizing the context.
			 */
			static void MD2Final (
								  uint8_t  digest[16],                         /* message digest */
								  MD2_CTX *context                                 /* context */
								  )
			{
				unsigned int index, padLen;
				
				/* Pad out to multiple of 16.
				 */
				index = context->count;
				padLen = 16 - index;
				MD2Update(context, PADDING[padLen], padLen);
				
				/* Extend with checksum */
				MD2Update(context, context->checksum, 16);
				
				/* Store state in digest */
				memcpy( digest, context->state, 16);
				
				/* Zeroize sensitive information.
				 */
				memset(context, 0, sizeof(*context));
			}
			
			
		} // RFC1319 
		
		md2::md2() throw() : function(__length, __window), ctx()
		{
		}
		
		const char *md2:: name() const throw()
		{
			return "MD2";
		}
		
		
		void md2:: set() throw()
		{
			RFC1319::MD2Init( &ctx );
		}
		
		void md2:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buflen>0&&NULL==buffer) );
			RFC1319::MD2Update( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void md2::get(void *output, size_t outlen ) throw()
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[16];
			RFC1319::MD2Final( digest, &ctx );
			fill( output, outlen, digest, sizeof(digest) );
		}
		
		md2:: ~md2() throw()
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}
		
		
	}
	
}
