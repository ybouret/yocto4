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
			
			atom * atom:: create( rule &r  )
			{
				return new (object::acquire1<atom>()) atom(r); //-- no throw after allocation
			}
			
			
			void atom:: destroy( atom *a ) throw()
			{
				assert( a );
				a->~atom();
				object::release1<atom>(a);
			}
			
			void atom:: destroy( atom *a, t_char::pool &tp ) throw()
			{
				assert( a );
				a->back_to( tp );
				destroy(a);
			}
			
			atoms:: atoms()  throw() {}
			atoms:: ~atoms() throw() { delete_with( atom::destroy ); }
			
			void atoms:: to( t_char::pool &tp ) throw()
			{ 
				delete_with<t_char::pool&>( atom::destroy, tp );
			}
			
			
		}
		
	}
	
}
