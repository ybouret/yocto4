#ifndef __DOUBLE_HOT_HPP_INCLUDED
#define __DOUBLE_HOT_HPP_INCLUDED 1
#include "yocto/gfx/color/ramp.hpp"
namespace yocto { namespace gfx {

	class double_hot : public ramp {
		public:
		explicit double_hot() throw();
		virtual ~double_hot() throw();
		private: YOCTO_DISABLE_COPY_AND_ASSIGN(double_hot);
	};

} }
#endif
