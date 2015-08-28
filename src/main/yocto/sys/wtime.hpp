#ifndef YOCTO_WTIME_INCLUDED
#define YOCTO_WTIME_INCLUDED 1

#include "yocto/sys/rt-clock.hpp"

namespace yocto
{
    
	class wtime : public rt_clock
	{
	public:
		explicit wtime();
		virtual ~wtime() throw();
		void   start();
		double query() const;
        
		static uint32_t seed();
        
        
		static void sleep( double s ) throw();
        
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(wtime);
		uint64_t last;
	};
    
#if 0
#define YOCTO_PERFORMANCE(SPEED,DURATION,CODE) \
do {              \
wtime  CHRONO;    \
size_t CYCLES = 0;\
CHRONO.start();   \
const uint64_t ini = CHRONO.ticks(); \
uint64_t       end = ini;             \
double         tmx = 0;               \
do { { CODE; } ++CYCLES; end = CHRONO.ticks(); } \
while( (tmx=CHRONO(end-ini)) < DURATION ); \
SPEED = double(CYCLES)/tmx;  \
} \
while(false)
#endif
    
}

#endif
