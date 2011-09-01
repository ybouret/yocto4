#include "yocto/utest/run.hpp"
#include "yocto/duration.hpp"


using namespace yocto;

static void show_duration( const double seconds )
{
	const duration d(seconds);
	std::cerr << seconds << " => " << d << std::endl;
}

#define SHOW(s) show_duration(s)

YOCTO_UNIT_TEST_IMPL(duration)
{
	SHOW(10.1);
	SHOW(59);
	SHOW(60);
	SHOW(1000);
	SHOW(10000);
	SHOW(100000);
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/code/rand.hpp"
#include "yocto/eta.hpp"

YOCTO_UNIT_TEST_IMPL(eta)
{
	eta ETA;
	double X = 0;
	ETA.reset();
	
	while( X < 1 )
	{
		wtime::sleep( 0.02 * alea<double>() );
		X += 0.01 * alea<double>();
		ETA(X);
		const duration d( ETA.time_left );
		std::cerr << d << "        \r";
		std::cerr.flush();
	}
	std::cerr << std::endl;
}
YOCTO_UNIT_TEST_DONE()

