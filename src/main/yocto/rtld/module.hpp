#ifndef YOCTO_RTLD_MODULE_INCLUDED
#define YOCTO_RTLD_MODULE_INCLUDED 1

#include "yocto/rtld/dylib.hpp"

namespace yocto
{
	//! wrapper for dylib
	class module 
	{
	public:
		explicit module( dylib *user_dll ) throw();
		virtual ~module() throw();
		
		module( const module &other ) throw();
		
		void *query( const string & ) const throw();
		void *query( const char   * ) const throw();
		
        template <typename T>
        inline void link( T &fn , const string &id) const
        {
            fn = T( query(id) );
        }
        
        template <typename T>
        inline void link( T &fn , const char *id) const
        {
            const string ID(id);
            link<T>(fn,ID);
        }

        
	private:
		YOCTO_DISABLE_ASSIGN(module);
		dylib *dll;
	};
	
}

#endif
