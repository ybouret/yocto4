#include "yocto/utest/run.hpp"
#include "yocto/error.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(platform)
{
	char buffer[256];
	libc::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Libc:  " << buffer << std::endl;
	win32::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Win32: " << buffer << std::endl;
}
YOCTO_UNIT_TEST_DONE()
