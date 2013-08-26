#include "yocto/utest/run.hpp"
#include "yocto/error.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/sys/hw.hpp"

using namespace yocto;

template <typename T>
static inline void test_endian()
{
	for( size_t i=0; i < 4; ++i )
	{
		T x = alea_of<T>();
		T y = swap_be<T>(x);
		T z = swap_be<T>(y);
		std::cerr << std::hex;
		
		std::cerr << x << " " << y << " " << z << std::endl;
	}
}

YOCTO_UNIT_TEST_IMPL(platform)
{
	std::cerr << "#CPU=" << hardware::nprocs() << std::endl;
	
	char buffer[256];
	libc::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Libc:  " << buffer << std::endl;
	win32::format_error( buffer, sizeof(buffer), 0 );
	std::cerr << "** Win32: " << buffer << std::endl;
	std::cerr << "-----------------------------------------------" << std::endl;
	for( size_t i=0; i < 3000; ++i )
	{
		if( is_a_power_of_two(i)) std::cerr << "pow2: " << i << std::endl;
	}
	
	std::cerr << "-----------------------------------------------" << std::endl;
	std::cerr << "next power of two:" << std::endl;
	size_t v = 1;
	for( size_t i=0; i < 30; ++i )
	{
		for( size_t j=0; j<8; ++j, v += (j+i) )
		{
			std::cerr << " (" << v <<","<<next_power_of_two(v) << ")";
		}
		std::cerr << std::endl;
	}
	
	std::cerr << "-----------------------------------------------" << std::endl;
	test_endian<uint16_t>();
	test_endian<uint32_t>();
	test_endian<uint64_t>();
	
}
YOCTO_UNIT_TEST_DONE()
