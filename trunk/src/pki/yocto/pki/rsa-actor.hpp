#ifndef YOCTO_PKI_RSA_ACTOR_INCLUDED
#define YOCTO_PKI_RSA_ACTOR_INCLUDED 1

#include "yocto/pki/rsa-auth.hpp"

namespace yocto
{
    namespace pki
    {
        class rsa_actor 
        {
        public:
            explicit rsa_actor() throw();
            virtual ~rsa_actor() throw();
            
            void delete_self_key() throw();
            void delete_peer_key() throw();
            
            void load_self_key( ios::istream &fp ); //!< fp must contain a private key
            void load_peer_key( ios::istream &fp ); //!< fp must contain a public key
            
            void load_self_key( const rsa_private_key &prv );
            void load_peer_key( const rsa_public_key  &pub );
            
            const rsa_key & self_prv() const;
            const rsa_key & self_pub() const;
            const rsa_key & peer_key() const;
            
        private:
            rsa_private_key *self_prv_;
            rsa_public_key  *self_pub_;
            rsa_public_key  *peer_key_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_actor);
        };
    }
}

#endif
