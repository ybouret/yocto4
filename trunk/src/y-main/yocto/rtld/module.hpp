#ifndef YOCTO_RTLD_MODULE_INCLUDED
#define YOCTO_RTLD_MODULE_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	class module : public object
	{
	public:
		explicit module( const string &soname );
		explicit module( const char   *soname );
		virtual ~module() throw();
		
		module( const module &other ) throw();
		
		void *query( const string & ) throw();
		void *query( const char   * );
		
        template <typename T>
        inline void link( T &fn , const string &id)
        {
            fn = T( query(id) );
        }
        
	private:
		YOCTO_DISABLE_ASSIGN(module);
		class  impl;
		impl  *impl_;
		
	};
	
}

#endif
