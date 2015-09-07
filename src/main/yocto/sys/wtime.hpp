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

}

#endif
