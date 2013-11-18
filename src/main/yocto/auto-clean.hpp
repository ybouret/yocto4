#ifndef YOCTO_AUTO_CLEAN_INCLUDED
#define YOCTO_AUTO_CLEAN_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	
	template <typename T>
	class auto_clean 
	{
	public:
		inline auto_clean( T &host, void (T::*method)(void) throw() ) throw() :
		host_( host ),
		method_( method ) { assert(method_!=NULL); }
		
		inline ~auto_clean() throw() { (host_.*method_)(); }
		
	private:
		T        &host_;
		void (T::*method_)(void) throw(); 
		YOCTO_DISABLE_COPY_AND_ASSIGN(auto_clean);
	};
	
}

#endif
