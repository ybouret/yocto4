#include "yocto/rx/lexical/rule.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace lexical
		{

			rule:: ~rule() throw()
			{
				assert( motif );
				delete motif;
			}
		
			rule:: rule( pattern *p, const string &l) :
			next(NULL),
			prev(NULL),
			motif( p ),
			label( l )
			{
				
			}
			
			void  rule:: destroy( rule *r ) throw()
			{
				assert( r );
				r->~rule();
				object::release1<rule>(r);
			}
		
		
			rule * rule::create( pattern *p, const string &l) 
			{
				//--------------------------------------------------------------
				// memory for rule
				//--------------------------------------------------------------
				rule *r = NULL;
				try 
				{
					r = object::acquire1<rule>();
				}
				catch(...)
				{
					delete p; 
					throw;
				}
				
				//--------------------------------------------------------------
				// controlled creation
				//--------------------------------------------------------------
				try 
				{
					new (r) rule( p, l );
				}
				catch(...)
				{
					object::release1<rule>(r);
					delete p;
					throw;
				}
				return r;
			}
			
		}

	}

}
