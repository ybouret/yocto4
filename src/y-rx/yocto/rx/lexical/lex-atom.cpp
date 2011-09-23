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
			
			atom * atom:: create( rule &r, atom::pool &ap )
			{
				atom *a = NULL;
				if( ap.size <= 0 )
				{
					a = object::acquire1<atom>();
				}
				else 
				{
					a = ap.query();
					memset( a, 0, sizeof(atom) );
				}

				return new (a) atom(r); //-- no throw
			}
			
			
			void atom:: destroy( atom *a ) throw()
			{
				assert( a );
				a->~atom();
				object::release1<atom>(a);
			}
			
			void atom:: destroy( atom *a, atom::pool &ap, t_char::pool &tp ) throw()
			{
				assert( a );
				a->back_to( tp );
				a->~atom();
				ap.store(a);
			}
			
			atom::pool::  pool() throw() {}
			atom::pool:: ~pool() throw() { delete_with( object::release1<atom> ); }
			
			atoms:: atoms()  throw() {}
			atoms:: ~atoms() throw() { delete_with( atom::destroy ); }
			
			void atoms:: to( atom::pool &ap, t_char::pool &tp ) throw()
			{ 
				delete_with<atom::pool&,t_char::pool&>( atom::destroy, ap, tp );
			}
			
			
		}

	}

}
