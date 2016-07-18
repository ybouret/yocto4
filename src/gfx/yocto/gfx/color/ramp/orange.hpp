#ifndef YOCTO_GRAPHICS_RAMP_ORANGE_HPP_INCLUDED
#define YOCTO_GRAPHICS_RAMP_ORANGE_HPP_INCLUDED 1
#include "yocto/gfx/color/ramp.hpp"
namespace yocto { namespace gfx{

	class orange : public ramp {
		public:
		explicit orange() throw();
		virtual ~orange() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(orange);
	};

} }
#endif
