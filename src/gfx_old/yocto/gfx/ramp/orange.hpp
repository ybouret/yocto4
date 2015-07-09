#ifndef YOCTO_GFX_RAMP_ORANGE_INCLUDED
#define YOCTO_GFX_RAMP_ORANGE_INCLUDED 1
#include "yocto/gfx/ramp.hpp"

namespace yocto { namespace gfx {
	class orange : public ramp {
	public:
		virtual ~orange() throw();
		explicit orange() throw();
	private: YOCTO_DISABLE_COPY_AND_ASSIGN(orange);
	};
}}
#endif
