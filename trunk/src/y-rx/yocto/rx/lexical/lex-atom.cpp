#include "yocto/rx/lexical/atom.hpp"
#include "yocto/rx/source.hpp"

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
			
			void atom:: destroy( atom *a, source &src ) throw()
			{
				assert( a );
				a->back_to( src.pool );
				delete a;
			}
			
			
			
			atoms:: atoms() throw() {}
			atoms:: ~atoms() throw() { delete_with( atom::destroy ); }
			
			void atoms:: to( source &src ) throw() { delete_with<source&>( atom::destroy, src); }
			
			
		}

	}

}
