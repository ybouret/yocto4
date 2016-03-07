#ifndef YOCTO_GRAPHICS_RAMP_GREY_HPP_INCLUDED
#define YOCTO_GRAPHICS_RAMP_GREY_HPP_INCLUDED 1
#include "yocto/graphics/ramp.hpp"
namespace yocto { namespace graphics{

	class grey : public ramp {
		public:
		explicit grey() throw();
		virtual ~grey() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(grey);
	};

} }
#endif
