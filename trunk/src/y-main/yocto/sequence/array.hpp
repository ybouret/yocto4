#ifndef YOCTO_SEQUENCE_ARRAY_INCLUDED
#define YOCTO_SEQUENCE_ARRAY_INCLUDED 1

#include "yocto/container/sequence.hpp"


namespace yocto
{
	
	namespace hidden
	{
		extern const char array_name[];
	}
	
	template <typename T>
	class array : public virtual container
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		virtual ~array() throw() {}
		
		inline type       & operator[]( size_t index ) throw()       { assert(index>0);assert(index<=size()); return *(type *)(get_item()+index); }
		inline const_type & operator[]( size_t index)  const throw() { assert(index>0);assert(index<=size()); return *(get_item()+index);         }
		
		
	protected:
		inline array() throw() {}
		
	private:
		virtual const_type *get_item() const throw() = 0; //!< for a 1..size access
	};
	
}

#endif
