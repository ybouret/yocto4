#include "yocto/pki/rsa-actor.hpp"


namespace yocto
{
    namespace pki
    {
        
        rsa_actor:: ~rsa_actor() throw()
        {
            delete_self_key();
            delete_peer_key();
        }
     
        rsa_actor:: rsa_actor() throw() :
        rsa_auth(),
        self_prv(0),
        self_pub(0),
        peer_key(0)
        {
            
        }
        
        void rsa_actor:: delete_self_key() throw()
        {
            if( self_prv )
            {
                assert(self_pub);
                delete self_prv;
                self_prv = 0;
                self_pub = 0;
            }
        }
        
        void rsa_actor:: delete_peer_key() throw()
        {
            if( peer_key )
            {
                delete peer_key;
                peer_key = 0;
            }
        }
        
        void rsa_actor:: load_self_key( ios::istream &fp )
        {
            rsa_private_key *key = rsa_private_key::load_prv(fp);
            delete_self_key();
            self_prv = key;
            self_pub = key;
        }
        
        void rsa_actor:: load_peer_key( ios::istream &fp )
        {
            rsa_public_key *key = rsa_public_key::load_pub(fp);
            delete_peer_key();
            peer_key = key;
        }
        
        void rsa_actor:: load_self_key( const rsa_private_key &prv )
        {
            rsa_private_key *key = new rsa_private_key( prv );
            delete_self_key();
            self_prv = key;
            self_pub = key;
        }

        void rsa_actor:: load_peer_key( const rsa_public_key  &pub )
        {
            rsa_public_key *key = new rsa_public_key(pub);
            delete_peer_key();
            peer_key = key;
        }
    }
}
