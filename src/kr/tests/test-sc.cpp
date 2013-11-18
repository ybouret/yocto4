#include "yocto/utest/run.hpp"
#include "yocto/crypto/sc/arc4.hpp"
#include "../../main/tests/support.hpp"
#include "yocto/random/uuid.hpp"
#include "yocto/crypto/sc/isaac-ks.hpp"

using namespace yocto;

namespace 
{
	struct encdec
	{
		crypto::key_stream *enc;
		crypto::key_stream *dec;
	};
	
}

YOCTO_UNIT_TEST_IMPL(sc)
{
	
	const uuid raw_key;
	const string key = raw_key.to_string();
	std::cerr << "Key=" << key << std::endl;
	
	crypto::arc4 enc_arc4( key );
	crypto::arc4 dec_arc4( key );
	
	crypto::isaac_key_stream enc_isaac( key );
	crypto::isaac_key_stream dec_isaac( key );
	
	encdec reg[] = { { &enc_arc4, &dec_arc4 }, { &enc_isaac, & dec_isaac } };
	const size_t num = sizeof(reg)/sizeof(reg[0]);
	
	
	for( size_t i=0; i < 8; ++i )
	{
		const string org = gen<string>::get();
		org.output( std::cerr ) << ": " << std::endl;
		for( size_t j=0; j < num; ++j )
		{
			encdec &C = reg[j];
			string enc = org;
			(*C.enc)( enc );
			string dec = enc;
			(*C.dec)( dec );
			dec.output_visible( std::cerr ) << "  <=  ";
			enc.output_visible( std::cerr ) << std::endl;
		}
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()

