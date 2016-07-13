#ifndef YOCTO_GRAPHICS_RAMP_BLUE_TO_RED_HPP_INCLUDED
#define YOCTO_GRAPHICS_RAMP_BLUE_TO_RED_HPP_INCLUDED 1
#include "yocto/gfx/color/ramp.hpp"
namespace yocto { namespace gfx{

	class blue_to_red : public ramp {
		public:
		explicit blue_to_red() throw();
		virtual ~blue_to_red() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(blue_to_red);
	};

} }
#endif
