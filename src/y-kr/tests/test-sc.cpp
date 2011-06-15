#include "yocto/utest/run.hpp"
#include "yocto/crypto/sc/arc4.hpp"

#include "../../y-main/tests/support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(sc)
{
	
	string key = argv[0]; key += gen<string>::get();
	crypto::arc4 encipher( key );
	crypto::arc4 decipher( key );
	
	for( size_t i=0; i < 8; ++i )
	{
		const string org = gen<string>::get();
		org.output( std::cerr ) << std::endl;
		string enc = org;
		encipher.cipher( enc );
		enc.output_visible( std::cerr ) << std::endl;
		string dec = enc;
		decipher.cipher( dec );
		dec.output( std::cerr ) << std::endl;
		std::cerr << std::endl;
	}
	
	
}
YOCTO_UNIT_TEST_DONE()

