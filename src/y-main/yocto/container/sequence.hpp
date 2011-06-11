#ifndef YOCTO_CONTAINER_SEQUENCE_INCLUDED
#define YOCTO_CONTAINER_SEQUENCE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/type-traits.hpp"

namespace yocto
{
	
	template <typename T>
	class sequence : public container
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		inline virtual ~sequence() throw() {}
		
		virtual void push_back( param_type ) = 0;
		virtual void pop_back() throw()      = 0;
		
		virtual void push_front( param_type ) = 0;
		virtual void pop_front() = 0;
		
		inline type       & front() throw() { return *(type *) &get_front(); }
		inline const_type & front() const throw() { return get_front(); }
		
	protected:
		inline explicit sequence() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(sequence);
		virtual const_type &get_front() const throw() = 0;
		virtual const_type &get_back()  const throw() = 0;
		
	};
	
}

#endif
