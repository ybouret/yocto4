#ifndef YOCTO_ASSOCIATIVE_INCLUDED
#define YOCTO_ASSOCIATIVE_INCLUDED 1

#include "yocto/container/container.hpp"

namespace yocto
{
	
	template <typename KEY, typename TYPE>
	class associative : public container
	{
	public:
		virtual ~associative() throw() {}
		
	protected:
		explicit associative() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(associative);
	};
	
}

#endif
