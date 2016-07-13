#ifndef YOCTO_GRAPHICS_RAMP_GREY_HPP_INCLUDED
#define YOCTO_GRAPHICS_RAMP_GREY_HPP_INCLUDED 1
#include "yocto/gfx/color/ramp.hpp"
namespace yocto { namespace gfx{

	class grey : public ramp {
		public:
		explicit grey() throw();
		virtual ~grey() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(grey);
	};

} }
#endif
