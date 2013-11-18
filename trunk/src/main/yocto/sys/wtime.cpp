#if defined(YOCTO_BSD)
#define _POSIX_C_SOURCE 199309 
#endif


#include "yocto/sys/wtime.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sys/pid.hpp"

#if defined(YOCTO_BSD)
#include <cmath>
#include <ctime>
#include <cerrno>
#endif


#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace yocto 
{

	wtime:: ~wtime() throw()
	{

	}

	wtime:: wtime( ) :
	last( ticks() )
	{
		
	}


	void wtime:: start()
	{
		calibrate();
		last = ticks();
	}

	double wtime:: query() const
	{
		const uint64_t now = ticks();
		return (*this)(now-last);
	}


	uint32_t wtime:: seed()
	{
		union 
		{
			uint64_t  ticks;
			uint32_t  dw[2];
		} ini = { ticks() };
#if defined(__INTEL_COMPILER)
#	pragma warning ( disable : 981 )
#endif
		const uint32_t data[4] = { ini.dw[0], ini.dw[1], ihash32( ini.dw[0] ) ^ ihash32( ini.dw[1] ), get_process_h32()  };

		uint32_t seed = hash32(data, sizeof(data) );
		return seed;
	}

	void wtime:: sleep( double seconds ) throw()
	{
		static const double sleep_min = 0;     /*          */
		static const double sleep_max = 86400; /*  one day */

		double s = clamp<double>(sleep_min,seconds,sleep_max);

#if defined(YOCTO_BSD)
		struct timespec info = { 0, 0 };
		struct timespec left = { 0, 0 };
		double num_secs      = 0;
		double num_nano      = modf(s,&num_secs);
		num_nano             = floor( 1.0e9 * num_nano );
		info.tv_sec  = (unsigned int)num_secs;
		info.tv_nsec = (unsigned int)num_nano;
		while( info.tv_nsec >= 1000000000 ) {
			++info.tv_sec;
			info.tv_nsec -= 1000000000;
		}
		while( nanosleep( &info, &left) < 0 ) {
			assert( errno == EINTR );
			info = left;
		}
#endif


#if defined(YOCTO_WIN)
		::Sleep( (DWORD)(s * 1000.0) );
#endif
	}

}
