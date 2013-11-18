#ifndef _YOCTO_GRAY_CIPHER_HPP_
#define _YOCTO_GRAY_CIPHER_HPP_ 1

#include "gray.hpp"
#include "endian.hpp"
#include "type-ints.hpp"
#include "block-cipher.hpp"
#include "ilog2.hpp"

namespace yocto {

	namespace crypto {



		template <
		const size_t               BITS,
		const block_cipher::action MODE
		>
		class gray_cipher : public block_cipher {
		public:
			static  const    size_t BYTES = BITS/8;
			typedef typename unsigned_int<BYTES>::type unit_type;

			explicit gray_cipher( const ro_buffer &key ) throw() :
					mask_(0)
			{
				//-- truncated key, padded with 0
				uint8_t *m = (uint8_t *)(void *)&mask_;
				for( size_t i=0; i < BYTES; ++i )
					m[i] = key.get_byte(i);

			}

			virtual ~gray_cipher() throw() {
			}


			virtual size_t size() const throw() {
				return sizeof( unit_type );
			}

			virtual void crypt( void *output, const void *input ) throw() {
				crypt( output, input, int2type<MODE>() );
			}

		private:
			unit_type     mask_;

			inline void crypt( void *output, const void *input, int2type<encrypting> ) throw() {
				unit_type wksp = 0;
				memcpy( &wksp, input, sizeof(unit_type) );
				unit_type temp = wksp^mask_;
				temp = swap_be( temp );
				wksp = swap_be( gray<unit_type>::encode( temp ) );
				memcpy( output, &wksp, sizeof(unit_type) );
			}

			inline void crypt( void *output, const void *input, int2type<decrypting> ) throw() {
				unit_type wksp = 0;
				memcpy( &wksp, input, sizeof(unit_type) );
				unit_type temp = swap_be(wksp);
				wksp = swap_be( gray<unit_type>::decode( temp )  ) ^ mask_;
				memcpy( output, &wksp, sizeof(unit_type) );
			}


			YOCTO_DISABLE_COPY_AND_ASSIGN(gray_cipher);
		};


		namespace gray8 {
			typedef gray_cipher<8,block_cipher::encrypting> encrypter;
			typedef gray_cipher<8,block_cipher::decrypting> decrypter;
		}

		namespace gray16 {
			typedef gray_cipher<16,block_cipher::encrypting> encrypter;
			typedef gray_cipher<16,block_cipher::decrypting> decrypter;
		}

		namespace gray32 {
			typedef gray_cipher<32,block_cipher::encrypting> encrypter;
			typedef gray_cipher<32,block_cipher::decrypting> decrypter;
		}

		namespace gray64 {
			typedef gray_cipher<64,block_cipher::encrypting> encrypter;
			typedef gray_cipher<64,block_cipher::decrypting> decrypter;
		}




	}


}

#endif
