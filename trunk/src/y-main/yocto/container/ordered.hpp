#ifndef YOCTO_CONTAINER_ORDERED_INCLUDED
#define YOCTO_CONTAINER_ORDERED_INCLUDED 1

#include "yocto/container/container.hpp"


namespace yocto
{
	
	template <typename T>
	class ordered : public container
	{
	public:
		virtual ~ordered() throw() {}
		
		
	protected:
		explicit ordered() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(ordered);
	};
	
}

#endif
