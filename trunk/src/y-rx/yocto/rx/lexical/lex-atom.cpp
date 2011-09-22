#include "yocto/rx/lexical/atom.hpp"

namespace yocto
{
	namespace regex
	{
		namespace lexical
		{
			
			atom:: ~atom() throw() 
			{
				
			}
			
			atom:: atom( rule &r ) throw() :
			token(),
			next(NULL),
			prev(NULL),
			label( r.label )
			{
				r.motif->swap_with( *this );
			}
			
			atom * atom:: create( rule &r )
			{
				return new atom( r );
			}
			
			
			void atom:: destroy( atom *a ) throw()
			{
				assert( a );
				delete a;
			}
			
			void atom:: destroy( atom *a, t_pool &p ) throw()
			{
				assert( a );
				a->back_to( p );
				delete a;
			}
			
			
			
			atoms:: atoms() throw() {}
			atoms:: ~atoms() throw() { delete_with( atom::destroy ); }
			
			void atoms:: to( t_pool &p ) throw() { delete_with<t_pool&>( atom::destroy, p); }
			
			
		}

	}

}
