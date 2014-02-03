#include "./wtime.hpp"

WallTime:: WallTime() throw() :
last(0)
{
}

WallTime:: ~WallTime() throw()
{
}

void WallTime:: start()
{
    last = Ticks();
}

double WallTime:: query() const
{
    const RunTimeClock &clk = *this;
    return clk( Ticks() - last );
}

uint32_t WallTime:: Seed()
{
	const union
	{
		uint64_t  ticks;
		uint32_t  dw[2];
	} ini = { RunTimeClock:: Ticks() };

	const uint32_t seed = IntegerHash32( ini.dw[0] ) ^ IntegerHash32( ini.dw[1] );
	return seed;
}



