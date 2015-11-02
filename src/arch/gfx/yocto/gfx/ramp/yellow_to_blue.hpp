#ifndef YOCTO_GFX_RAMP_YELLOW_TO_BLUE_INCLUDED
#define YOCTO_GFX_RAMP_YELLOW_TO_BLUE_INCLUDED 1
#include "yocto/gfx/ramp.hpp"

namespace yocto { namespace gfx {
	class yellow_to_blue : public ramp {
	public:
		virtual ~yellow_to_blue() throw();
		explicit yellow_to_blue() throw();
	private: YOCTO_DISABLE_COPY_AND_ASSIGN(yellow_to_blue);
	};
}}
#endif
