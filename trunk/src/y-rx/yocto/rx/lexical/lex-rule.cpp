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
			
			
			bool rule:: keep(YOCTO_RX_LEX_RULE_ACTION_ARGS) throw()
			{
				return true;
			}
			
			
			rule:: rule( pattern *p, const string &l) :
			next(NULL),
			prev(NULL),
			motif( p ),
			label( l ),
			check( this, & rule::keep )
			{
				
			}
			
			rule:: rule( pattern *p, const string &l, const action &a) :
			next(NULL),
			prev(NULL),
			motif( p ),
			label( l ),
			check( a )
			{
				
			}
			
			void  rule:: destroy(  rule *r ) throw()
			{
				assert( r );
				r->~rule();
				object::release1<rule>(r);
			}
			
			
			rule * rule::create( pattern *p, const string &l, const action *a ) 
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
					if( a )
						new (r) rule( p, l, *a );
					else 
						new (r) rule( p, l);
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
