#include "yocto/pki/rsa-actor.hpp"
#include "yocto/exception.hpp"

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
        self_prv_(0),
        self_pub_(0),
        peer_key_(0)
        {
            
        }
        
        void rsa_actor:: delete_self_key() throw()
        {
            if( self_prv_ )
            {
                assert(self_pub_);
                delete self_prv_;
                self_prv_ = 0;
                self_pub_ = 0;
            }
        }
        
        void rsa_actor:: delete_peer_key() throw()
        {
            if( peer_key_ )
            {
                delete peer_key_;
                peer_key_ = 0;
            }
        }
        
        void rsa_actor:: load_self_key( ios::istream &fp )
        {
            rsa_private_key *key = rsa_private_key::load_prv(fp);
            delete_self_key();
            self_prv_ = key;
            self_pub_ = key;
        }
        
        void rsa_actor:: load_peer_key( ios::istream &fp )
        {
            rsa_public_key *key = rsa_public_key::load_pub(fp);
            delete_peer_key();
            peer_key_ = key;
        }
        
        void rsa_actor:: load_self_key( const rsa_private_key &prv )
        {
            rsa_private_key *key = new rsa_private_key( prv );
            delete_self_key();
            self_prv_ = key;
            self_pub_ = key;
        }

        void rsa_actor:: load_peer_key( const rsa_public_key  &pub )
        {
            rsa_public_key *key = new rsa_public_key(pub);
            delete_peer_key();
            peer_key_ = key;
        }
        
        const rsa_key & rsa_actor:: self_prv() const
        {
            if( !self_prv_ ) throw exception("rsa_actor self key is not loaded");
            return *self_prv_;
        }
        
        const rsa_key & rsa_actor:: self_pub() const
        {
            if( !self_pub_ ) throw exception("rsa_actor self key is not loaded");
            return *self_pub_;
        }
        
        const rsa_key & rsa_actor:: peer_key() const
        {
            if( !peer_key_ ) throw exception("rsa_actor peer key is not loaded");
            return *peer_key_;
        }

    }
}
