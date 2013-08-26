/** \file pid.hpp
 \brief access the process identifier
 */

#ifndef YOCTO_SYS_PID_INCLUDED
#define YOCTO_SYS_PID_INCLUDED 1

#include "yocto/os.hpp"

#if defined(YOCTO_BSD)
#	include <unistd.h>
#endif

namespace yocto
{
#if defined(YOCTO_BSD)
	typedef pid_t   pid_type;
#endif
	
#if defined(YOCTO_WIN)
	typedef uint32_t pid_type;
#endif
	
	
	pid_type get_process_id();
	uint32_t get_process_h32();
    
	
}
#endif

