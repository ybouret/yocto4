#include "yocto/mpk/natural.hpp"


namespace yocto
{
	
	namespace mpk
	{
		int natural:: compare( const natural &lhs, const natural &rhs ) throw()
		{
			if( lhs.size_ < rhs.size_ )
			{
				assert(rhs.byte_[rhs.size_-1] > 0 );
				return -1;
			}
			else 
			{
				if( lhs.size_ > rhs.size_ )
				{
					assert(lhs.byte_[lhs.size_-1]>0);
					return 1;
				}
				else 
				{
					assert( lhs.size_ == rhs.size_ );
					register size_t n = lhs.size_;
					const uint8_t *pL = lhs.byte_+n;
					const uint8_t *pR = rhs.byte_+n;
					for( ;n>0;--n)
					{
						const ptrdiff_t L = *(--pL);
						const ptrdiff_t R = *(--pR);
						if( L < R )
							return -1;
						else
						{
							if( L > R )
								return 1;
							else
							{
								assert( L == R );
								continue;
							}
						}
					}
					return 0; 
				}
			}
		}
		
#		define YMPN_CMP_IMPL(FUNC) \
/*    */	bool operator FUNC ( const natural &lhs, const natural &rhs ) throw() \
/*    */	{ return natural::compare(lhs,rhs) FUNC 0; }
		
		YMPN_CMP_IMPL(==)
		YMPN_CMP_IMPL(!=)
		YMPN_CMP_IMPL(<)
		YMPN_CMP_IMPL(<=)
		YMPN_CMP_IMPL(>)
		YMPN_CMP_IMPL(>=)
		
	}
	
}
