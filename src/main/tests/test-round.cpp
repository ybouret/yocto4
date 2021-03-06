#include "yocto/utest/run.hpp"
#include "yocto/code/round.hpp"
#include "yocto/threading/mutex.hpp"

using namespace yocto;
using namespace threading;


#define __TEST_ALIGN_SIZE(BYTES,N) std::cerr << "Align " << N << " on " << BYTES << " : " << YOCTO_ALIGN_FOR_SIZE(BYTES,N) << std::endl
#define __TEST_ALIGN_ITEM(T,N)     std::cerr << "Align " << N << " on " << #T    << " : " << YOCTO_ALIGN_FOR_ITEM(T,N) << std::endl

YOCTO_UNIT_TEST_IMPL(round)
{
	for( size_t i=0; i < 10; ++i )
	{
		std::cerr << i;
		for( size_t j=0; j < 8; ++j )
		{
			std::cerr << " " << YOCTO_ROUND(j,i);
		}
		std::cerr << std::endl;
	}
	std::cerr << "sizeof(mutex)=" << sizeof(mutex) << std::endl;
	std::cerr << "#u64         =" << YOCTO_U64_FOR_ITEM(mutex) << std::endl;
	uint64_t dummy[ YOCTO_U64_FOR_ITEM(mutex) ];
	(void)dummy;
	std::cerr << "sizeof(dummy)=" << sizeof(dummy) << std::endl;

    for(size_t bits=0;bits<=33;++bits)
    {
        std::cerr << "#Bits= " << bits << " => " << YOCTO_BYTES_FOR(bits) << " bytes" << std::endl;
    }
    
    std::cerr << std::endl;
    __TEST_ALIGN_SIZE(1,9);
    __TEST_ALIGN_SIZE(2,13);
    __TEST_ALIGN_SIZE(4,6);
    __TEST_ALIGN_SIZE(8,13);
    
    std::cerr << std::endl;
    __TEST_ALIGN_ITEM(char,17);
    __TEST_ALIGN_ITEM(short,19);
    __TEST_ALIGN_ITEM(size_t,11);
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/math/round.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(log_round)
{
    string        line;
    ios::icstream fp( ios::cstdin );
    while( line.clear(), ( std::cerr << "> ").flush(), fp.read_line(line) >= 0 )
    {
        const double dt = strconv::to_double( line.c_str(), "dt" );
        std::cerr << "dt=" << dt << std::endl;
        if( dt > 0 )
        {
            std::cerr << "log_round_floor=" << math::log_round_floor(dt) << std::endl;
            std::cerr << "log_round_ceil =" << math::log_round_ceil(dt) << std::endl;

        }
        else
            std::cerr << "invalid dt<=0" << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
