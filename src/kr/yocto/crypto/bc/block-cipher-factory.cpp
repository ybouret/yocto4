#include "yocto/crypto/bc/block-cipher-factory.hpp"
#include "yocto/exception.hpp"

#include "yocto/crypto/bc/block-cipher-ecb.hpp"
#include "yocto/crypto/bc/block-cipher-cbc.hpp"
#include "yocto/crypto/bc/block-cipher-ctr.hpp"
#include "yocto/crypto/bc/block-cipher-cfb.hpp"
#include "yocto/crypto/bc/block-cipher-ofb.hpp"
#include "yocto/crypto/bc/block-cipher-pcbc.hpp"

namespace yocto
{
    namespace crypto
    {
        
        block_cipher_factory:: ~block_cipher_factory() throw()
        {
        }
        
        
        
        //======================================================================
        // ECB
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_ecb(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ecb::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_ecb(block_cipher &e,
                                                block_cipher &d,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ecb::decrypter(d,e,iv);
            }
            
            
        }
        
        //======================================================================
        // CBC
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_cbc(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_cbc::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_cbc(block_cipher &e,
                                                block_cipher &d,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_cbc::decrypter(d,e,iv);
            }
            
            
        }
        
        //======================================================================
        // CTR
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_ctr(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ctr::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_ctr(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ctr::decrypter(e,iv);
            }
            
            
        }
        
        //======================================================================
        // CFB
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_cfb(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_cfb::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_cfb(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_cfb::decrypter(e,iv);
            }
            
            
        }
        
        //======================================================================
        // OFB
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_ofb(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ofb::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_ofb(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_ofb::decrypter(e,iv);
            }
            
            
        }

        //======================================================================
        // PCBC
        //======================================================================
        namespace
        {
            static inline
            operating_block_cipher *encrypt_pcbc(block_cipher &e,
                                                block_cipher &,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_pcbc::encrypter(e,iv);
            }
            
            static inline
            operating_block_cipher *decrypt_pcbc(block_cipher &e,
                                                block_cipher &d,
                                                const memory::ro_buffer &iv )
            {
                return new block_cipher_pcbc::decrypter(d,e,iv);
            }
            
            
        }

        
        
        
        void block_cipher_factory:: declare( const string &id, creator proc)
        {
            if( !db.insert(id,proc) )
                throw exception("block_cipher_library(multiple '%s')", id.c_str());
        }
        
        void block_cipher_factory:: declare( const char *id, creator proc)
        {
            const string ID(id);
            declare(ID,proc);
        }
        
        const char block_cipher_factory::name[] = "bc_factory";
        
        block_cipher_factory:: block_cipher_factory() :
        db()
        {
            
#define Y_BCF(ID) declare(#ID,ID)
            
            Y_BCF(encrypt_ecb);
            Y_BCF(decrypt_ecb);
            
            Y_BCF(encrypt_cbc);
            Y_BCF(decrypt_cbc);
            
            Y_BCF(encrypt_ctr);
            Y_BCF(decrypt_ctr);
            
            Y_BCF(encrypt_cfb);
            Y_BCF(decrypt_cfb);
            
            Y_BCF(encrypt_ofb);
            Y_BCF(decrypt_ofb);
            
            Y_BCF(encrypt_pcbc);
            Y_BCF(decrypt_pcbc);
            
        }
        
        
        size_t block_cipher_factory:: size() const throw()
        {
            return db.size();
        }
        
        block_cipher_factory::iterator block_cipher_factory::begin() const throw()
        {
            return db.begin();
        }
        
        block_cipher_factory::iterator block_cipher_factory::end() const throw()
        {
            return db.end();
        }
        
        
        operating_block_cipher * block_cipher_factory:: create( const string &id, block_cipher &e, block_cipher &d, const memory::ro_buffer &iv) const
        {
            const creator *hook = db.search(id);
            if(!hook)
                throw exception("%s.create( no '%s')", name, id.c_str());
            creator proc = *hook; assert(proc);
            return proc(e,d,iv);
        }
    }
}
