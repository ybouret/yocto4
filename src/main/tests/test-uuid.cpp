#include "yocto/utest/run.hpp"
#include "yocto/random/uuid.hpp"
#include "yocto/sequence/list.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(uuid)
{
	list<uuid> keys;
	
	for( size_t i=0; i < 16; ++i )
	{
		const uuid tmp;
		keys.push_back( tmp );
	}
	
	for( list<uuid>::iterator i = keys.begin(); i != keys.end(); ++i )
	{
		const string str = (*i).to_string();
		std::cerr << str << std::endl;
	}
	
}
YOCTO_UNIT_TEST_DONE()
