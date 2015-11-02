#ifndef YOCTO_GFX_RAMP_DOUBLE_HOT_INCLUDED
#define YOCTO_GFX_RAMP_DOUBLE_HOT_INCLUDED 1
#include "yocto/gfx/ramp.hpp"

namespace yocto { namespace gfx {
	class double_hot : public ramp {
	public:
		virtual ~double_hot() throw();
		explicit double_hot() throw();
	private: YOCTO_DISABLE_COPY_AND_ASSIGN(double_hot);
	};
}}
#endif
