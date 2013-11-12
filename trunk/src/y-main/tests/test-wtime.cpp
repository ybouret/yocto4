#include "yocto/utest/run.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(wtime)
{
	wtime chrono;

	chrono.start();
	for( size_t i=0; i < 10; ++i )
	{
		std::cerr << "#ticks=" << wtime::ticks() << std::endl;
	}
	const double ell = chrono.query();
	std::cerr << "In " << 1e6 * ell << " microsec" << std::endl;
    std::cerr << std::endl;
    const double ini = 1.0e6*chrono.query();
    const double tmx = 1.0e6*chrono.query()  - ini;
    std::cerr << "tmx=" << tmx << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sys/rt-clock.hpp"

YOCTO_UNIT_TEST_IMPL(clock)
{
    rt_clock chrono;
    
    const uint64_t ini = chrono.ticks();
	const uint64_t fin = chrono.ticks();
    const uint64_t tmx = fin-ini;
	std::cerr << "ini=" << ini << std::endl;
	std::cerr << "fin=" << fin << std::endl;
    std::cerr << "tmx=" << tmx << std::endl;
    std::cerr << "ell=" << chrono(tmx) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
