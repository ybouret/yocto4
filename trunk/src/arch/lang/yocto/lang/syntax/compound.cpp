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
            
                    
            void compound:: operator()( rule &r )
            {
                operands.push_back( &r );
            }

            compound & compound:: operator<<( rule &r )
            {
                (*this)(r);
                return *this;
            }
		}
	}

}
