#ifndef YOCTO_ETA_INCLUDED
#define YOCTO_ETA_INCLUDED 1

#include "yocto/sys/wtime.hpp"

namespace yocto
{
	class eta
	{
	public:
		explicit eta();
		virtual ~eta() throw();
		
		const double time_done;
		const double time_left;
		const double ratio_done;
		const double ratio_left;
		
		void   reset();                    //!< initialize values and timer
		double now();                      //!< query timer, values untouched 
		void   operator()( double ratio ); //!< update all
		
	private:
		wtime chrono_;
		YOCTO_DISABLE_COPY_AND_ASSIGN(eta);
	};
}

#endif
