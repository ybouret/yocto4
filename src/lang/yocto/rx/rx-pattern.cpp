#include "yocto/rx/pattern.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/osstream.hpp"

namespace yocto
{
	namespace regex
	{
		pattern:: pattern( uint32_t t ) throw() : 
		type(t), 
		data(NULL),
		next(NULL),
		prev(NULL)
		{
		}
		
		pattern:: ~pattern() throw() 
		{
		}
		
		void pattern:: clear() throw()
		{
			this->release();
		}
		
		void pattern:: optimize() 
		{
		}
		
        bool pattern:: is_same( const pattern &lhs, const pattern &rhs )
        {
            string L,R;
            ios::osstream Lfp( L );
            ios::osstream Rfp( R );
            lhs.brx( Lfp );
            rhs.brx( Rfp );
            return L == R;
        }
		
			
	}
}

