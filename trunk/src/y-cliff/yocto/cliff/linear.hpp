#ifndef YOCTO_CLIFF_LINEAR_INCLUDED
#define YOCTO_CLIFF_LINEAR_INCLUDED 1


#include "yocto/cliff/layout.hpp"


namespace yocto
{
	
	template <typename T, typename LAYOUT>
	class linear : public LAYOUT
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		const size_t bytes;
		T           *entry; //!< must be linked to an array of LAYOUT::items
		
		
		virtual ~linear() throw() {}
		
		explicit linear( const LAYOUT &L ) throw() :
		LAYOUT( L ),
		bytes( this->items * sizeof(T) ),
		entry( NULL )
		{
		}
		
		virtual void link( T *addr ) throw() = 0;
		
		void ldz() throw() { assert(entry); memset( entry, 0, bytes); }
		
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
	};
	
}

#endif
