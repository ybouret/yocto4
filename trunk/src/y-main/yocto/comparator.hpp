#ifndef YOCTO_COMPARATOR_INCLUDED
#define YOCTO_COMPARATOR_INCLUDED

#include "yocto/type-traits.hpp"

namespace yocto
{
	template <typename T>
	class comparator
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline  comparator() throw() {}
		inline ~comparator() throw() {}
		
		inline int operator()( param_type lhs, param_type rhs ) const throw() 
		{
			return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0 );
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(comparator);
	};
	
	
}

#endif
