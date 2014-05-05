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
    
    B.free();
    B.push_full<uint8_t>(1);
    B.output(std::cerr,B.size()); std::cerr << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/code/binary.hpp"
template <typename T>
static inline void __test_reverse_bits( int nmax )
{
    for(int i=0;i<nmax;++i)
    {
        const T x(i);
        const T y = reverse_bits(x);
        const T z = reverse_bits(y);
        if(z!=x)
            throw exception("Invalid %d bits reverse_bits", int(8*sizeof(T)) );
    }
}

YOCTO_UNIT_TEST_IMPL(binary)
{
    __test_reverse_bits<uint8_t>(256);
    __test_reverse_bits<uint16_t>(65536);
    __test_reverse_bits<uint32_t>(100000);
    __test_reverse_bits<uint64_t>(100000);    
    
}
YOCTO_UNIT_TEST_DONE()
