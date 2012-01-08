#ifndef YOCTO_CONTAINER_INCLUDED
#define YOCTO_CONTAINER_INCLUDED 1

#include "yocto/object.hpp"

#if !defined(YOCTO_CONTAINER_CAPACITY_LN2)
#define	YOCTO_CONTAINER_CAPACITY_LN2 4
#endif

namespace yocto
{
	
	class container : public object
	{
	public:
		virtual ~container() throw();
		
		virtual const char *name() const throw() = 0;
		virtual size_t      size()     const throw() = 0;
		virtual size_t      capacity() const throw() = 0;
		virtual void        reserve(size_t n) = 0;
		virtual void        free()    throw() = 0;
		virtual void        release() throw() = 0;
		
		static size_t next_capacity( size_t n );
		static size_t next_increase( size_t n );
		size_t available() const throw();
		void   ensure( size_t n );
					  
	protected:
		explicit container() throw();
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(container);
	};
	
}

#endif
