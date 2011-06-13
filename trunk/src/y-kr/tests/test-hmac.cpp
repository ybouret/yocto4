#include "yocto/utest/run.hpp"
#include "yocto/hashing/hash-mac.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;

typedef hashing::hmac<hashing::md5>  hmac_md5;
typedef hashing::hmac<hashing::sha1> hmac_sha1;

YOCTO_UNIT_TEST_IMPL(hmac)
{
	//--------------------------------------------------------------
	// HMAC-MD5
	//--------------------------------------------------------------
	{
		std::cerr << "test1" << std::endl;
		const digest key(16,0x0b);
		const string data ="Hi There";
		const string auth = "9294727a3638bb1c13f48ef8158bfc9d";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc, data );
		std::cerr << dp << std::endl;
		
	}
	
	{
		std::cerr << "test2" << std::endl;
		const string key  = "Jefe";
		const string data = "what do ya want for nothing?";
		const string auth = "750c783e6ab0b503eaa86e310a5db738";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp =digest::checksum( hc, data );
		std::cerr << dp << std::endl;
		
	}
	
	{
		std::cerr << "test3" << std::endl;
		const digest key(16,0xaa);
		const digest data(50,0xdd);
		const string auth = "56be34521d144c88dbb8c733f0e8b3f6";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
		
	}
	
	
	{
		std::cerr << "test4" << std::endl;
		const digest key = digest::hex("0102030405060708090a0b0c0d0e0f10111213141516171819");
		const digest data(50,0xcd);
		const string auth = "697eaf0aca3a3aea3a75164746ffaa79";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
		
	}
	
	{
		std::cerr << "test5" << std::endl;
		const digest key(16,0x0c);
		const string data ="Test With Truncation";
		const string auth = "56461ef2342edc00f9bab995690efd4c";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
	{
		std::cerr << "test6" << std::endl;
		const digest key(80,0xaa);
		const string data = "Test Using Larger Than Block-Size Key - Hash Key First";
		const string auth = "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd";
		std::cerr << auth << std::endl;
		
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
	{
		std::cerr << "test7" << std::endl;
		const digest key(80,0xaa);
		const string data = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
		const string auth = "6f630fad67cda0ee1fb1f562db3aa53e";
		std::cerr << auth << std::endl;
		
		hmac_md5  hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
		
	}
	
	
	//--------------------------------------------------------------
	// HMAC-SHA1
	//--------------------------------------------------------------
	
	{
		std::cerr << "test1" << std::endl;
		const digest key(20,0x0b);
		const string data = "Hi There";
		const string auth = "b617318655057264e28bc0b6fb378c8ef146be00";
		std::cerr << auth << std::endl;
		
		hmac_sha1 hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
		
	}
	
	{
		std::cerr << "test2" << std::endl;
		const string key  = "Jefe";
		const string data = "what do ya want for nothing?";
		const string auth = "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79";
		std::cerr << auth << std::endl;
		
		hmac_sha1 hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
	{
		std::cerr << "test3" << std::endl;
		const digest key(20,0xaa);
		const digest data(50,0xdd);
		const string auth = "125d7342b9ac11cd91a39af48aa17b4f63f175d3";
		std::cerr << auth << std::endl;
		
		hmac_sha1 hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
	{
		std::cerr << "test4" << std::endl;
		const digest key = digest::hex("0102030405060708090a0b0c0d0e0f10111213141516171819");
		const digest data(50,0xcd);
		const string auth = "4c9007f4026250c6bc8414f9bf50c86c2d7235da";
		std::cerr << auth << std::endl;
		
		hmac_sha1 hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
	{
		std::cerr << "test5" << std::endl;
		const digest key(20,0x0c);
		const string data = "Test With Truncation";
		const string auth = "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04";
		std::cerr << auth << std::endl;
		
		hmac_sha1 hc( key );
		const digest dp = digest::checksum( hc,  data );
		std::cerr << dp << std::endl;
	}
	
}
YOCTO_UNIT_TEST_DONE()
