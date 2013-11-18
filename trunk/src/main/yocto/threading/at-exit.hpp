//! \file

#ifndef YOCTO_THREADING_AT_EXIT_INCLUDED
#define YOCTO_THREADING_AT_EXIT_INCLUDED 1

#include "yocto/os.hpp"

#if !defined(YOCTO_AT_EXIT_STACK_SIZE)
#define YOCTO_AT_EXIT_STACK_SIZE 64
#endif

namespace yocto
{
	namespace threading
	{
		typedef int longevity;
		
		struct at_exit
		{
			typedef void (*callback)(void);
			static void perform( at_exit::callback, threading::longevity ) throw();
		};
		
		
	}
}

#endif
