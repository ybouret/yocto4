#include "yocto/utest/run.hpp"
#include "yocto/crypto/bc/aes.hpp"
#include "yocto/crypto/bc/teac.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/hashing/md2.hpp"
//#include "yocto/string/string-io.hpp"

#include "yocto/crypto/bc/block-cipher-ecb.hpp"
#include "yocto/crypto/bc/block-cipher-cbc.hpp"
#include "yocto/crypto/bc/block-cipher-cfb.hpp"
#include "yocto/crypto/bc/block-cipher-ofb.hpp"
#include "yocto/crypto/bc/block-cipher-pcbc.hpp"

#include "yocto/crypto/sc/arc4.hpp"
#include "yocto/random/uuid.hpp"

#include "yocto/ios/icstream.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace crypto;





static
void process_all(memory::blender            &p,
				 memory::blender            &q,
				 const memory::ro_buffer    &instr,
				 const string              &line )
{
	string enc = line;
	
	p.schedule( instr );
	p( enc   );
	
	string dec = enc;
	q.schedule( instr );
	q( dec   );
	
#if 0
	std::cerr << "org=[";
	display_line( line );
	std::cerr << "]" << std::endl;
#endif
	
	std::cerr << " [";
	enc.output_visible( std::cerr );
	std::cerr << "]";
	
	if( dec != line )
		std::cerr << "\tFAILURE";// << std::endl;
	else
		std::cerr << "\tSUCCESS";// << std::endl;
}

static
void process_pair(block_cipher &e,
                  block_cipher &d,
                  const string &line,
                  const memory::ro_buffer &IV )
{
	{
		block_cipher_ecb::encrypter e_ecb( e, IV );
		block_cipher_ecb::decrypter d_ecb( d, e, IV );
		process_all( e_ecb, d_ecb, IV, line );
		std::cerr << " ECB-" << e.size() * 8 << std::endl;
	}
	
	{
		block_cipher_cbc::encrypter e_cbc( e, IV );
		block_cipher_cbc::decrypter d_cbc( d, e, IV );
		process_all( e_cbc, d_cbc, IV, line );
		std::cerr << " CBC-" << e.size() * 8 << std::endl;
	}
	
	{
		block_cipher_cfb::encrypter e_cfb( e, IV );
		block_cipher_cfb::decrypter d_cfb( e, IV );
		process_all( e_cfb, d_cfb, IV, line );
		std::cerr << " CFB-" << e.size() * 8 << std::endl;
	}
	
	{
		block_cipher_ofb::encrypter e_ofb( e, IV );
		block_cipher_ofb::decrypter d_ofb( e, IV );
		process_all( e_ofb, d_ofb, IV, line );
		std::cerr << " OFB-" << e.size() * 8 << std::endl;
	}
	
	
	{
		
		block_cipher_pcbc::encrypter e_pcbc( e, IV );
		block_cipher_pcbc::decrypter d_pcbc( d, e, IV );
		process_all( e_pcbc, d_pcbc, IV, line );
		std::cerr << " PCBC-" << e.size() * 8 << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(bc)
{
	const uuid   raw_key;
	const string key = raw_key.to_string();
	const digest usr_key = checksum<hashing::md5>( key );
	const digest usr_iv  = checksum<hashing::md2>( key );
	
	aes128::encrypter e_aes128( usr_key );
	aes128::decrypter d_aes128( usr_key );
	
	aes192::encrypter e_aes192( usr_key );
	aes192::decrypter d_aes192( usr_key );
	
	aes256::encrypter e_aes256( usr_key );
	aes256::decrypter d_aes256( usr_key );
	
	teac_of<64>::encrypter e_tea64( usr_key );
	teac_of<64>::decrypter d_tea64( usr_key );
	
	teac_of<100>::encrypter e_tea100( usr_key );
	teac_of<100>::decrypter d_tea100( usr_key );
	
	arc4 e_arc4( usr_key );
	
	//-- auto test
	string line;
	for( size_t i=0; i < 64; ++i )
	{
		line += char( 'a'+alea_lt(25) );
		process_pair( e_aes128, d_aes128, line, usr_iv );
		process_pair( e_aes192, d_aes192, line, usr_iv );
		process_pair( e_aes256, d_aes256, line, usr_iv );
		process_pair( e_tea64 , d_tea64,  line, usr_iv );
		process_pair( e_tea100, d_tea100, line, usr_iv );
		process_all(  e_arc4,   e_arc4, usr_key, line  ); std::cerr << " ARC4" << std::endl;
	}
	
	
#if 0
	ios::icstream input( ios::cstdin );
	while( input.gets( line ) && line != ".quit" )
	{
		
		process_pair( e_aes128, d_aes128, line, usr_iv );
		process_pair( e_aes192, d_aes192, line, usr_iv );
		process_pair( e_aes256, d_aes256, line, usr_iv );
		process_pair( e_tea64 , d_tea64,  line, usr_iv );
		process_pair( e_tea100, d_tea100, line, usr_iv );
		process_all(  e_arc4,   e_arc4, usr_key, line  ); std::cerr << " ARC4" << std::endl;
	}
#endif
	
}
YOCTO_UNIT_TEST_DONE()
