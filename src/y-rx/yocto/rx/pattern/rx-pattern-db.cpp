#include "yocto/rx/pattern/db.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	namespace regex
	{
			
		pattern_db::  pattern_db() throw() {}
		pattern_db:: ~pattern_db() throw() {}
		
		static const char fn[] = "regex::pattern_db";
		void pattern_db::record( const string &name, pattern *p )
		{
			// smart shared pointer:: p is delete upon failure
			pattern_ptr pp( p ); 
			
			if( ! pattern_map::insert( name, pp ) )
			{
				throw exception("%s(multiple '%s')", fn, name.c_str() );
			}
		}
		
		void pattern_db:: record( const char   *name, pattern *p )
		{
			pattern_ptr  pp( p ); 
			const string id(name);
			
			if( ! pattern_map::insert( name, pp ) )
			{
				throw exception("%s(multiple '%s')", fn, name );
			}
			
		}
		
		pattern * pattern_db:: create( const string &name ) const
		{
			const pattern_ptr *ppPat = search( name );
			if( ! ppPat ) 
				throw exception("%s(no '%s')", fn, name.c_str() );
			return (*ppPat)->clone();
		}
		
		pattern * pattern_db:: create( const char *name ) const
		{
			const string id( name );
			return create( id );
		}
		
		
	}
}
