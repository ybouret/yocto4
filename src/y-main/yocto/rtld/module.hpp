#ifndef YOCTO_RTLD_MODULE_INCLUDED
#define YOCTO_RTLD_MODULE_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	class module : public object
	{
	public:
		explicit module( const string &soname );
		virtual ~module() throw();
		
		module( const module &other ) throw();
		
		void *query( const string & ) throw();
		void *query( const char   * );
		
	private:
		YOCTO_DISABLE_ASSIGN(module);
		class  impl;
		impl  *impl_;
		
	};
	
}

#endif
