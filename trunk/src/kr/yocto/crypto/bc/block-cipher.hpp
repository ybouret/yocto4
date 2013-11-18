#ifndef _YOCTO_BLOCK_CIPHER_HPP_
#define _YOCTO_BLOCK_CIPHER_HPP_ 1

#include "yocto/hashing/digest.hpp"

namespace yocto {

	namespace crypto {

		class block_cipher : public virtual object 
		{
			public:
				enum action { encrypting, decrypting };

				virtual ~block_cipher() throw();

				virtual size_t        size() const throw() = 0; //!< block size
				virtual void          crypt( void *output, const void *input ) throw() = 0; //!< blocks of this->size() !

			protected:
				explicit block_cipher() throw();


			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(block_cipher);
		};

	}

}

#endif
