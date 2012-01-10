#include "yocto/cliff/level-set.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
	namespace cliff
	{
		
		using math::real_t;
		
		////////////////////////////////////////////////////////////////////////
		template<>
		level_set<real_t>:: level:: ~level() throw() {}
		
		template<>
		level_set<real_t>:: level:: level( param_key id, param_type v ) :
		key(  id ),
		value( v )
		{
		}
		
		template<>
		level_set<real_t>:: level::   level( const level &other ) :
		key(   other.key   ),
		value( other.value )
		{
		}
		
		////////////////////////////////////////////////////////////////////////
		template <>
		level_set<real_t>:: ~level_set() throw() {}
		
		template <>
		level_set<real_t>:: level_set() throw() : levels_() {}
		
		template <>
		level_set<real_t>::level_set( size_t n ) : levels_(n,as_capacity) {}
		
		template <>
		void level_set<real_t>:: add( param_key key, param_type value )
		{
			//--------------------------------------------------------------
			// check no multiple keys
			//--------------------------------------------------------------
			for( size_t i=levels_.size();i>0;--i)
				if( key == levels_[i].key )
					throw libc::exception( EINVAL, "level_sets.add(multiple key '%d')",key);
			
			//--------------------------------------------------------------
			// insert: check no multiple values
			//--------------------------------------------------------------
			const level tmp( key, value );
			if( ! levels_.insert( tmp ) )
				throw libc::exception( EINVAL, "level_sets.add(multiple value '%g')",value);
			
		}
		
	}
}
