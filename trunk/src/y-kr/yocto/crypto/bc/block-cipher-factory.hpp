#ifndef YOCTO_CRYPTO_BC_FACTORY_INCLUDED
#define YOCTO_CRYPTO_BC_FACTORY_INCLUDED

#include "yocto/crypto/bc/operating-block-cipher.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/threading/singleton.hpp"

namespace yocto
{
    namespace crypto
    {
        class block_cipher_factory : public singleton<block_cipher_factory>
        {
        public:
            typedef operating_block_cipher * (*creator)( block_cipher &e, block_cipher &d, const memory::ro_buffer &iv );
            typedef map<string,creator>        database;
            
            virtual ~block_cipher_factory() throw();
            
            
            void declare( const string &id, creator proc);
            void declare( const char   *id, creator proc);
            
            size_t size() const throw();
            
            typedef database::const_iterator iterator;
            iterator begin() const throw();
            iterator end()   const throw();
            
            operating_block_cipher * create( const string &id, block_cipher &e, block_cipher &d, const memory::ro_buffer &iv) const;
            
        private:
            explicit block_cipher_factory();
            database db;
            friend class singleton<block_cipher_factory>;
            static const char                 name[];
            static const threading::longevity life_time = 19; // TODO: chek
            YOCTO_DISABLE_COPY_AND_ASSIGN(block_cipher_factory);
        };
    }
}


#endif
