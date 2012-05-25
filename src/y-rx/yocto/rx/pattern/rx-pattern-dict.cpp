#include "yocto/rx/pattern/dict.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	namespace regex
	{
			
		pattern_dict::  pattern_dict() throw() {}
		pattern_dict:: ~pattern_dict() throw() {}
		
		static const char fn[] = "regex::pattern_db";
        
		void pattern_dict::record( const string &name, pattern *p )
		{
			// smart shared pointer:: p is delete upon failure
			pattern_ptr pp( p ); 
			
			if( ! pattern_map::insert( name, pp ) )
			{
				throw exception("%s(multiple '%s')", fn, name.c_str() );
			}
		}
		
		void pattern_dict:: record( const char   *name, pattern *p )
		{
			pattern_ptr  pp( p ); 
			const string id(name);
			
			if( ! pattern_map::insert( id, pp ) )
			{
				throw exception("%s(multiple '%s')", fn, name );
			}
			
		}
		
		pattern * pattern_dict:: create( const string &name ) const
		{
			const pattern_ptr *ppPat = search( name );
			if( ! ppPat ) 
				throw exception("%s(no '%s')", fn, name.c_str() );
			return (*ppPat)->clone();
		}
		
		pattern * pattern_dict:: create( const char *name ) const
		{
			const string id( name );
			return create( id );
		}
		
        void pattern_dict:: record( const char   *name, const char *expr)
        {
            pattern *p = compile(expr,this);
            record(name,p);
        }
		
	}
}
