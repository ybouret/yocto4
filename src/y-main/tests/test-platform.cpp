#include "yocto/utest/run.hpp"
#include "yocto/error.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(platform)
{
	char buffer[256];
	libc::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Libc:  " << buffer << std::endl;
	win32::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Win32: " << buffer << std::endl;
	for( size_t i=0; i < 3000; ++i )
	{
		if( is_a_power_of_two(i)) std::cerr << "pow2: " << i << std::endl;
	}
	std::cerr << "next power of two:" << std::endl;
	size_t v = 0;
	for( size_t i=0; i < 30; ++i )
	{
		for( size_t j=0; j<8; ++j, v += (j+i) )
		{
			std::cerr << " (" << v <<","<<next_power_of_two(v) << ")";
		}
		std::cerr << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()
