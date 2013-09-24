#ifndef YOCTO_CRYPTO_BC_FACTORY_INCLUDED
#define YOCTO_CRYPTO_BC_FACTORY_INCLUDED

#include "yocto/crypto/bc/operating-block-cipher.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace crypto
    {
        class block_cipher_factory
        {
        public:
            typedef operating_block_cipher * (*creator)( block_cipher &e, block_cipher &d, const memory::ro_buffer &iv );
            typedef map<string,creator>        database;
            
            explicit block_cipher_factory();
            virtual ~block_cipher_factory() throw();
            
            
            void declare( const string &name, creator proc);
            void declare( const char   *name, creator proc);
            
            size_t size() const throw();
            
            typedef database::const_iterator iterator;
            iterator begin() const throw();
            iterator end()   const throw();
            
        private:
            database db;
            YOCTO_DISABLE_COPY_AND_ASSIGN(block_cipher_factory);
        };
    }
}


#endif
