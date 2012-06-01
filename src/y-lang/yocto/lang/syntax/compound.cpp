#include "yocto/lang/syntax/compound.hpp"

namespace yocto 
{
	namespace lang
	{

		namespace syntax
		{
			compound:: ~compound() throw() {}

			compound:: compound( const string &id ) :
			rule( id )
			{
			}

		}
	}

}