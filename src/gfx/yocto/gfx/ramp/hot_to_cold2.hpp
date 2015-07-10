#ifndef YOCTO_GFX_RAMP_HOT_TO_COLD2_INCLUDED
#define YOCTO_GFX_RAMP_HOT_TO_COLD2_INCLUDED 1
#include "yocto/gfx/ramp.hpp"

namespace yocto { namespace gfx {
	class hot_to_cold2 : public ramp {
	public:
		virtual ~hot_to_cold2() throw();
		explicit hot_to_cold2() throw();
	private: YOCTO_DISABLE_COPY_AND_ASSIGN(hot_to_cold2);
	};
}}
#endif
