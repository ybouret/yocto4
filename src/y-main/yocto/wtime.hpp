#ifndef YOCTO_WTIME_INCLUDED
#define YOCTO_WTIME_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{

	class wtime
	{
	public:
		explicit wtime() throw();
		virtual ~wtime() throw();
		void   start();
		double query() const;
		static uint64_t ticks();
		static uint32_t seed();
		
		static void sleep( double s ) throw();
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(wtime);
        
#if defined(YOCTO_BSD)
		int64_t data[2];
#endif

#if defined(YOCTO_WIN)
		int64_t      quad;
		long double  freq;
#endif
	};

}

#endif
