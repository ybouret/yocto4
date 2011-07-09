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
		
	private:
		YOCTO_DISABLE_ASSIGN(module);
		struct impl;
		impl  *impl_;
		
	};
	
}

#endif
