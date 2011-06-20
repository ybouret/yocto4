#ifndef YOCTO_ASSOCIATIVE_INCLUDED
#define YOCTO_ASSOCIATIVE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/type-traits.hpp"

namespace yocto
{
	
	template <typename KEY, typename T>
	class associative : public container
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		virtual ~associative() throw() {}
		
		virtual bool insert( param_key , param_type ) = 0;
		
	protected:
		explicit associative() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(associative);
	};
	
}

#endif
