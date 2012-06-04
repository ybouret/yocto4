#include "yocto/lang/syntax/compound.hpp"

namespace yocto 
{
	namespace lang
	{

		namespace syntax
		{
			compound:: ~compound() throw() {}

			compound:: compound( const string &id ) :
			rule( id ),
			operands()
			{
			}
            
            void compound:: add( rule *r ) throw()
            {
                assert(r);
                operands.push_back(r);
            }
            
            void compound:: add( const rule &r )
            {
                operands.push_back( r.clone() );
            }
            
            compound:: compound( const compound &other ) :
            rule( other ),
            operands( other.operands )
            {
            }
            

		}
	}

}
