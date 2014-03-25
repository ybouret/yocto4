#include "yocto/utest/run.hpp"
#include "yocto/pki/rsa-codec.hpp"
#include "yocto/random/isaac.hpp"
#include <cstdio>

using namespace yocto;

//typedef mpk::natural mpn;
using namespace pki;

YOCTO_UNIT_TEST_IMPL(rsa1)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	
	
	std::cerr << "sizeof(rsa_public_key)  = " << sizeof( rsa_public_key ) << std::endl;
	std::cerr << "sizeof(rsa_private_key) = " << sizeof( rsa_private_key ) << std::endl;
	
	
	const size_t pbits = 30;
	mpn prime1 = gen.rand<uint64_t>(pbits);
	mpn prime2 = gen.rand<uint64_t>(pbits);
	std::cerr << "Generating prime1" << std::endl;
	prime1 = prime1.next_prime_();
	
	std::cerr << "Generating prime2" << std::endl;
	prime2 = prime2.next_prime_();
	if( prime1 < prime2 )
		prime1.xch(prime2);
	
	std::cerr << "prime1=" << prime1 << std::endl;
	std::cerr << "prime2=" << prime2 << std::endl;
	
	const mpn modulus  = prime1 * prime2;
	mpn       p1m      = prime1; --p1m;
	mpn       p2m      = prime2; --p2m;
	const mpn z        = p1m * p2m;
	mpn publicExponent = gen.full<uint32_t>();
	if( !publicExponent.is_odd() )
		++publicExponent;
	while( !mpn::are_coprime(publicExponent,z) )
		publicExponent.inc(2);
	
	std::cerr << "publicExponent  = " << publicExponent << std::endl;
	
	const rsa_public_key pub( modulus, publicExponent );
	
	const mpn privateExponent = mpn::mod_inv(publicExponent,z);
	std::cerr << "privateExponent = " << privateExponent << std::endl;
	const mpn exponent1       = privateExponent % p1m;
	const mpn exponent2       = privateExponent % p2m;
	const mpn coefficient     = mpn::mod_inv( prime2, prime1 );
	std::cerr << "exponent1       = " << exponent1   << std::endl;
	std::cerr << "exponent2       = " << exponent2   << std::endl;
	std::cerr << "coefficient     = " << coefficient << std::endl;
	
	const rsa_private_key prv( modulus, publicExponent, privateExponent, prime1, prime2, exponent1, exponent2, coefficient );
	
	std::cerr << "pub ibits=" << pub.ibits << std::endl;;
    std::cerr << "pub obits=" << pub.obits << std::endl;;
    
	for( int i=0; i < 8; ++i )
	{
		const mpn P = mpn( gen.full<uint64_t>() ) % modulus;
		std::cerr << "P=" << P << " "; std::cerr.flush();
		const mpn C = pub.compute( P );
		std::cerr << "C=" << C << " "; std::cerr.flush();
		const mpn D = prv.compute( C );
		std::cerr << "D=" << D;
		
		std::cerr << std::endl;
		if( P != D )
			throw exception("RSA error...");
		
	}
    for( int i=0; i < 8; ++i )
	{
		const mpn P = mpn( gen.full<uint32_t>() ) % modulus;
		std::cerr << "P=" << P << " "; std::cerr.flush();
		const mpn C = pub.compute( P );
		std::cerr << "C=" << C << " "; std::cerr.flush();
		const mpn D = prv.compute( C );
		std::cerr << "D=" << D;
		
		std::cerr << std::endl;
		if( P != D )
			throw exception("RSA error...");
		
	}
    
	std::cerr << std::endl;
	
    std::cerr << "Modulus=" << pub.modulus << "/" << pub.modulus.bits() << std::endl;
	{
		std::cerr << "-- codec: encoding" << std::endl;
		std::cerr.flush();
		
        string encoded;
        char C;
        
        
        {
            const rsa_key::pointer pk( new rsa_private_key( prv ) );
            rsa_encoder encoder( pk );
            
            encoder("Hello, World!");
            encoder.flush();
            encoder("Y");
            encoder.flush();
            while( encoder.query(C) )
            {
                fprintf( stderr, "%02X", uint8_t(C) );
                encoded.append( C );
            }
            fprintf( stderr, "\n"); fflush(stderr);
        }
        
        std::cerr << "-- codec: decoding" << std::endl;
        {
            const rsa_key::pointer pk( new rsa_public_key( pub ) );
            rsa_decoder decoder( pk );
            for( size_t i=0; i < encoded.size(); ++i )
            {
                decoder.write( encoded[i] );
                while( decoder.query(C) )
                {
                    std::cerr << make_visible(C);
                }
            }
            
            std::cerr << std::endl;
        }
        
        
	}
	
	
}
YOCTO_UNIT_TEST_DONE()

#if defined(_MSC_VER)
#pragma warning ( disable : 4309 )
#endif

static const uint8_t keys_db[] =
{
#include "rsa-keys.h"
};

#include "yocto/ios/imstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/icstream.hpp"

using namespace mpk;

YOCTO_UNIT_TEST_IMPL(rsa2)
{
    vector<rsa_key::pointer>  pub_keys,prv_keys;
    {
        ios::imstream fp( keys_db, sizeof(keys_db) );
        char C;
        while( fp.query(C) )
        {
            fp.store(C);
            rsa_public_key        *pub = rsa_public_key:: load_pub(fp);
            const rsa_key::pointer pk( pub );
            pub_keys.push_back( pk );
            std::cerr << "+pub.key@" << pub->obits << " => " << pub->obits/8 << " bytes" << std::endl;
        }
    }
    
    {
        ios::imstream fp( keys_db, sizeof(keys_db) );
        char C;
        while( fp.query(C) )
        {
            fp.store(C);
            rsa_private_key       *prv = rsa_private_key:: load_prv(fp);
            const rsa_key::pointer pk( prv );
            prv_keys.push_back( pk );
            std::cerr << "+prv.key@" << prv->obits << std::endl;
        }
    }
    
    std::cerr << "Waiting for input...." << std::endl;
    ios::icstream fp( ios::cstdin );
    string line;
    while( fp.read_line( line ) > 0 && line != ".quit" )
    {
        std::cerr << "--------" << std::endl;
        for( size_t i=1; i <= pub_keys.size(); ++i )
        {
            const rsa_key::pointer &prv_k = prv_keys[i];
            const rsa_key::pointer &pub_k = pub_keys[i];
            if( prv_k->obits<= 512 )
            {
                string crypted;
                rsa_encoder enc( prv_k );
                std::cerr << "encoding: "; std::cerr.flush();
                enc << line;
                enc.flush();
                char C;
                while( enc.query(C) )
                {
                    crypted.append(C);
                    std::cerr << make_visible(C);
                }
                std::cerr << std::endl;
                std::cerr << "decoding: "; std::cerr.flush();
                rsa_decoder dec( pub_k );
                dec << crypted;
                dec.flush();
                while( dec.query(C) )
                {
                    std::cerr << make_visible(C);
                }
                std::cerr << std::endl;
            }
            
        }
        
        std::cerr << "--------" << std::endl;
        line.clear();
    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/pki/rsa-auth.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/string/base64.hpp"

YOCTO_UNIT_TEST_IMPL(auth)
{
    
    vector<rsa_key::pointer>  pub_keys,prv_keys;
    {
        ios::imstream fp( keys_db, sizeof(keys_db) );
        char C;
        while( fp.query(C) )
        {
            fp.store(C);
            rsa_public_key        *pub = rsa_public_key:: load_pub(fp);
            const rsa_key::pointer pk( pub );
            pub_keys.push_back( pk );
            std::cerr << "+pub.key@" << pub->obits << " => " << pub->obits/8 << " bytes" << std::endl;
        }
    }
    
    {
        ios::imstream fp( keys_db, sizeof(keys_db) );
        char C;
        while( fp.query(C) )
        {
            fp.store(C);
            rsa_private_key       *prv = rsa_private_key:: load_prv(fp);
            const rsa_key::pointer pk( prv );
            prv_keys.push_back( pk );
            std::cerr << "+prv.key@" << prv->obits << std::endl;
        }
    }
    std::cerr << "Reserve memory" << std::endl;
    rsa_auth auth(1024);
    const size_t nk = pub_keys.size();
    std::cerr << "Loaded " << nk << " keys" << std::endl;
    std::cerr << "Enter Keys...." << std::endl;
    ios::icstream fp( ios::cstdin );
    string   line;
    
    
    const rsa_key  &self_prv_k = *prv_keys[2];
    const rsa_key  &self_pub_k = *pub_keys[2];
    
    const rsa_key  &peer_pub_k = *pub_keys[1];
    const rsa_key  &peer_prv_k = *prv_keys[1];
    
    hashing::sha1   alg;
    
    base64::encoder b64;
    
    while( fp.read_line( line ) > 0 && line != ".quit" )
    {
        
        std::cerr << "encrypt..." << std::endl;
        const string msg = auth.encrypt( line, peer_pub_k);
        const string m64 = b64.to_string(msg);
        std::cerr << m64 << std::endl;
        std::cerr << "signing..." << std::endl;
        string sgn = auth.signature( line, self_prv_k, alg );
        const string s64 = b64.to_string( sgn );
        std::cerr << s64 << std::endl;
        std::cerr << "decrypt..." << std::endl;
        const string dec = auth.decrypt( msg, peer_prv_k );
        std::cerr << "dec=" << dec << std::endl;
        const string hash_dec = rsa_auth::hash_string( dec, alg );
        const string hash_sgn = auth.decrypt( sgn, self_pub_k );
        if( hash_dec != hash_sgn )
        {
            std::cerr << "Mismatch!" << std::endl;
        }
        else
        {
            std::cerr << "Key is authenticated!" << std::endl;
        }
        std::cerr << "...done" << std::endl;
        line.clear();
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
