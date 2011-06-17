#ifndef YOCTO_ASSOCIATIVE_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_MAP_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
	
	namespace hidden
	{
		extern const char map_name[];
	}
	
	template <
	typename KEY,
	typename T, 
	typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global >
	class map : public associative<KEY,T>
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		//! data key/obj node
		class dnode
		{
		public:
			dnode     *next;
			dnode     *prev;
			const_key  key;
			T          obj;
		private:
			inline  dnode( param_key k, param_type o ) : next(NULL), prev(NULL), key(k), obj(0) {}
			inline ~dnode() throw() {}
			YOCTO_DISABLE_COPY_AND_ASSIGN(dnode);
		};
	
	
	
		
		explicit map() throw() {}
		virtual ~map() throw() {}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw() { return hidden::map_name; }
		
	private:
		KEY_HASHER hash_;
		ALLOCATOR  hmem_;
	};
	
}

#endif
