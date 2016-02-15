#ifndef YOCTO_SYS_RT_CLOCK_INCLUDED
#define YOCTO_SYS_RT_CLOCK_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	class rt_clock
	{
	public:
#if defined(YOCTO_APPLE) || defined(YOCTO_WIN)
        typedef double freq_type;
#else
        typedef uint64_t fre_type;
#endif
		explicit rt_clock();
		virtual ~rt_clock() throw();

		static uint64_t  ticks();     //!< system ticks
		void             calibrate(); //!< called during constructor, needed to transforms ticks to seconds.
		double           operator()( uint64_t num_ticks) const;
        inline freq_type frequency() const throw()
        {
            return freq;
        }

	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(rt_clock);
        freq_type freq;
    };

}

#endif

