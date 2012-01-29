#include "yocto/cliff/components.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	namespace cliff
	{
		
		components:: ~components() throw()
		{
		}
		
		static inline size_t __check( size_t length )
		{
			if( length <= 0 )
				throw exception("cliff.components(number=0)");
			return length;
		}
		
		
		components:: components( size_t num, const char *name_list[] ) :
		number( __check(num) ),
		names(number,as_capacity),
		idxDB(number+1,as_capacity)
		{
			if( name_list )
			{
				for( size_t i=0; i < number; ++i )
				{
					const string id( name_list[i] );
					if( !idxDB.insert( id, i+1 ) )
						throw exception("cliff.components: multiple provided '%s'", id.c_str());
					names.push_back( id );
				}
			}
			else
			{
				for( size_t i=1; i <= number; ++i )
				{
					const string tmp( vformat("%u", unsigned(i) ) );
					names.push_back( tmp );
					if(!idxDB.insert( tmp, i ))
						throw exception("cliff.components: unexpected multiple '%s'", tmp.c_str());
				}
			}
		}
		
		
		const string & components:: name( size_t i ) const throw()
		{
			assert( i >= 1 );
			assert( i <= number );
			return names[i];
		}
		
		const char * components:: text( size_t i ) const throw()
		{
			return name(i).c_str();
		}
		
		void components:: name( size_t i, const string &id )
		{
			assert( i >= 1);
			assert( i <= number );
			if( id == name(i) )
				return;
			
			if( idxDB.search(id) )
				throw exception("cliff.components.set_name(%u,%s): already in use", unsigned(i), id.c_str());
			
			string &current_id = names[i];
			
			if( !idxDB.insert(id,i) )
				throw exception("cliff.components.set_name(%u,%s): unexpected insert failure", unsigned(i), id.c_str());
			
			if( !idxDB.remove(current_id) )
				throw exception("cliff.components.set_name(%u,%s): unexpected remove failure", unsigned(i), id.c_str());
			
			current_id = id;
			
		}
		
		
		void components:: name( size_t i, const char *id )
		{
			const string _id(id);
			name(i,_id);
		}
		
		size_t components:: operator()( const string &id ) const
		{
			const size_t *p = idxDB.search( id );
			if( !p ) throw exception("no cliff.components['%s']", id.c_str() );
			return *p;
		}
		
		size_t components:: operator()( const char *id ) const
		{
			const string _id(id);
			return (*this)(_id);
		}
		
	}
	
	
}
