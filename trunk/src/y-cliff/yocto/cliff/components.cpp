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
				throw exception("cliff.components(empty name_list)");
			return length;
		}
		
		
		components:: components( size_t a, size_t b, const char *name_list[] ) :
		cmin( name_list ? a : min_of(a,b) ),
		cmax( name_list ? (a + __check(b))-1 : max_of(a,b) ),
		size( cmax+1 - cmin ),
		names(size,as_capacity),
		idxDB(size+1,as_capacity)
		{
			if( name_list )
			{
				assert(b>0);
				for( size_t i=0; i < b; ++i )
				{
					const string id( name_list[i] );
					if( !idxDB.insert( id, cmin+i ) )
						throw exception("cliff.components: multiple provided '%s'", id.c_str());
					names.push_back( id );
				}
			}
			else
			{
				for( size_t i=cmin; i <= cmax; ++i )
				{
					const string tmp( vformat("%u", unsigned(i) ) );
					names.push_back( tmp );
					if(!idxDB.insert( tmp, i ))
						throw exception("cliff.components: unexpected multiple '%s'", tmp.c_str());
				}
			}
		}
		
		
		const string & components:: get_name( size_t i ) const throw()
		{
			assert( i >= cmin );
			assert( i <= cmax );
			return names[++i-cmin];
		}
		
		const char * components:: get_text( size_t i ) const throw()
		{
			return get_name(i).c_str();
		}
		
		void components:: set_name( size_t i, const string &id )
		{
			assert( i >= cmin );
			assert( i <= cmax );
			if( idxDB.search(id) )
				throw exception("cliff.components.set_name(%u,%s): already in use", unsigned(i), id.c_str());
			
			string &current_id = names[i];
			
			if( !idxDB.insert(id,i) )
				throw exception("cliff.components.set_name(%u,%s): unexpected insert failure", unsigned(i), id.c_str());
			
			if( !idxDB.remove(current_id) )
				throw exception("cliff.components.set_name(%u,%s): unexpected remove failure", unsigned(i), id.c_str());
			
			current_id = id;
			
		}
		
		
		void components:: set_name( size_t i, const char *id )
		{
			const string _id(id);
			set_name(i,_id);
		}
		
		size_t components:: operator[]( const string &id ) const
		{
			const size_t *p = idxDB.search( id );
			if( !p ) throw exception("no cliff.components['%s']", id.c_str() );
			return *p;
		}
		
		size_t components:: operator[]( const char *id ) const
		{
			const string _id(id);
			return (*this)[_id];
		}
		
	}
	
	
}
