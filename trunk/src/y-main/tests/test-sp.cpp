#include "yocto/utest/run.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/string.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(shared_ptr)
{
	shared_ptr<string> pS1( new string("Hello") );
	shared_ptr<string> pS2( pS1 ); 

	std::cerr << *pS1 << std::endl;
	std::cerr << *pS2 << std::endl;
	
	*pS1 = "World";

	std::cerr << *pS1 << std::endl;
	std::cerr << *pS2 << std::endl;


}
YOCTO_UNIT_TEST_DONE()
