#include "yocto/pki/rsa-actor.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/string/base64.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;

static const uint8_t two_keys_data[] =
{
#include "two-keys.h"
};

YOCTO_UNIT_TEST_IMPL(actors)
{
    
    base64::encoder enc;
    pki::rsa_actor Alice;
    pki::rsa_actor Bob;
    pki::rsa_auth  auth;
    Random::Bits  &gen = Random::CryptoBits();
    string         session_key;
    
    if( argc > 1 )
        session_key = argv[1];
    else
    {
        for( size_t i=8 + gen.lt<size_t>(16);i>0;--i)
        {
            enc.write( gen.full<char>() );
        }
        enc.flush();
        session_key = enc.to_string();
    }
    
    // Alice prv=first key
    // Bob   prv=second key
    {
        ios::imstream fp( two_keys_data, sizeof(two_keys_data) );
        Alice.load_self_key(fp);
        Alice.load_peer_key(fp);
    }
    
    {
        ios::imstream fp( two_keys_data, sizeof(two_keys_data) );
        Bob.load_peer_key(fp);
        Bob.load_self_key(fp);
    }
    
    
  
    std::cerr << "Session Key     = " << session_key << std::endl;
    hashing::sha1 H;
    
    //! Alice Encrypts Session Key with Bob's public key
    const string AliceCodedKey   = auth.encrypt( session_key, Alice.peer_key() );
    const string AliceCodedKey64 = enc.to_string( AliceCodedKey );
    std::cerr << "AliceCodedKey   = " << AliceCodedKey64 << std::endl;
    
    //! Alice Signs The Session Key with it's private Key
    const string AliceSignature   = auth.signature( session_key, Alice.self_prv(), H);
    const string AliceSignature64 = enc.to_string(AliceSignature);
    std::cerr << "AliceSignature  = " << AliceSignature64 << std::endl;

    //! Bob Decrypts the Session Key with its private key
    const string BobPlainKey = auth.decrypt( AliceCodedKey, Bob.self_prv());
    std::cerr << "Bob Session Key = " << BobPlainKey << std::endl;
    
    if( BobPlainKey != session_key )
        throw exception("Invalid Decryption");
    
    //! Bob Decrypts Alice's Signature wiht Alice's public key
    const string BobSignature   = auth.decrypt( AliceSignature, Bob.peer_key() );
    const string BobSignature64 = enc.to_string(BobSignature);
    std::cerr << "BobSignature    = " << BobSignature64 << std::endl;
    const string BobHash        = auth.hash_string( BobPlainKey, H);
    const string BobHash64      = enc.to_string(BobHash);
    std::cerr << "BobHash         = " << BobHash64 << std::endl;
    
    if( BobHash != BobSignature )
        throw exception("Invalid Signature");
    std::cerr << "Session Key is Authenticated" << std::endl;
}
YOCTO_UNIT_TEST_DONE()
