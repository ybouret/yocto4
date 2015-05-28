#include "yocto/utest/run.hpp"
#include "yocto/code/property.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ppty)
{
	std::cerr << "int ppty" << std::endl;
	
	int my_int = 5;
	
	property<int> I( my_int, false,"I" );
	
	std::cerr << "read ppty" << std::endl;
	int j = I;
	std::cerr << "j=" << j << std::endl;
	
	std::cerr << "bool ppty" << std::endl;

	bool my_sending = true;
	
	property<bool> sending( my_sending, true, "sending");
	
	std::cerr << "sending=" << ( sending ?  "true" : "false" ) << std::endl;
	sending = false;
	std::cerr << "sending=" << ( sending ?  "true" : "false" ) << std::endl;

	std::cerr << "write ppty" << std::endl;
	I = 10;
	
	
}
YOCTO_UNIT_TEST_DONE()
