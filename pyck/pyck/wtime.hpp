#ifndef PYCK_WTIME_INCLUDED
#define PYCK_WTIME_INCLUDED 1

#include "./rt-clock.hpp"

class WallTime : public RunTimeClock
{
public:
	explicit WallTime() throw();
	virtual ~WallTime() throw();
    
	void   start();
	double query() const;
	
	static uint32_t Seed();

private:
	WallTime( const WallTime & );
	WallTime &operator=( const WallTime & );
    uint64_t last;
};

#endif
