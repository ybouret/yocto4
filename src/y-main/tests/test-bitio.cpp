#include "yocto/utest/run.hpp"
#include "yocto/ios/bitio.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bitio)
{

	ios::bitio B;
	
	std::cerr << "Filling..." << std::endl;
	B.push<int>(9,5);
	while( B.size() & 7 ) B.push( false );
	
	
	while( B.size() )
	{
		std::cerr << "x=" << B.pop<int>(8) << std::endl;
	}
	
}
YOCTO_UNIT_TEST_DONE()
