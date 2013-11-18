#ifndef YOCTO_ASSOCIATIVE_INCLUDED
#define YOCTO_ASSOCIATIVE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/type/key.hpp"

namespace yocto
{
	
	template <typename KEY, typename T>
	class associative : public container
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		virtual ~associative() throw() {}
		
		virtual bool insert( param_key , param_type ) = 0;
		virtual bool remove( param_key ) throw()      = 0;
		
		inline type       * search( param_key key ) throw() { return (type *)lookup(key); }
		inline const_type * search( param_key key ) const throw() { return   lookup(key); }
		
	protected:
		explicit associative() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(associative);
		virtual const_type *lookup( param_key ) const throw() = 0;
	};
	
}

#endif
