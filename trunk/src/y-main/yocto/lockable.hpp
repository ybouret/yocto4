#ifndef YOCTO_LOCKABLE_INCLUDED
#define YOCTO_LOCKABLE_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto 
{
	class lockable
	{
	public:
		virtual ~lockable() throw();
		virtual void lock()     throw() = 0;
		virtual void unlock()   throw() = 0;
		virtual bool try_lock() throw() = 0;
	protected:
		explicit lockable() throw();
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(lockable);
	};
}

#endif
