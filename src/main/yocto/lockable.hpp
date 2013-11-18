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
	
	class scoped_lock
	{
	public:
		explicit scoped_lock( lockable &host ) throw();
		virtual ~scoped_lock() throw();
		
	private:
		lockable &host_;
		YOCTO_DISABLE_COPY_AND_ASSIGN(scoped_lock);
	};
	
#define YOCTO_LOCK__(ID) __yocto_lock ## ID
#define YOCTO_LOCK_(ID) YOCTO_LOCK__(ID)
#define YOCTO_LOCK(LOCKABLE) volatile scoped_lock YOCTO_LOCK_(__LINE__)(LOCKABLE)
	
}

#endif
