#ifndef YOCTO_GRAPHICS_RAMP_COLD_TO_VERY_HOT_HPP_INCLUDED
#define YOCTO_GRAPHICS_RAMP_COLD_TO_VERY_HOT_HPP_INCLUDED 1
#include "yocto/gfx/color/ramp.hpp"
namespace yocto { namespace gfx{

	class cold_to_very_hot : public ramp {
		public:
		explicit cold_to_very_hot() throw();
		virtual ~cold_to_very_hot() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(cold_to_very_hot);
	};

} }
#endif
