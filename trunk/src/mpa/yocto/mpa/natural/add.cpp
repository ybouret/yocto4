#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace mpa
    {
        natural natural:: add( const natural &lhs, const natural &rhs )
        {
            
            //------------------------------------------------------------------
            // Organize so that L is the largest one
            //------------------------------------------------------------------
            const natural *L = & lhs;
            const natural *R = & rhs;
            if( R->size> L->size) cswap(L,R);
            assert( L->size >= R->size);
            
            const size_t   top_size = L->size;
            const size_t   com_size = R->size;
            natural        ans( top_size+1, as_capacity );
            const uint8_t *l = L->byte;
            const uint8_t *r = R->byte;
            uint8_t       *s = ans.byte;
            
            
            //------------------------------------------------------------------
			// Initialize
			//------------------------------------------------------------------
			unsigned carry = 0;
            
            //------------------------------------------------------------------
			// common loop
			//------------------------------------------------------------------
            size_t   i     = 0;
			for( ; i < com_size; ++i )
			{
				carry += l[i] + r[i];
				s[i]   = uint8_t(carry);
				carry >>= 8;
			}
			
            //------------------------------------------------------------------
			// top loop
			//------------------------------------------------------------------
			for( ; i < top_size; ++i )
			{
				carry += l[i];
				s[i]   = uint8_t(carry);
				carry >>= 8;
			}

            //------------------------------------------------------------------
			// finalize
			//------------------------------------------------------------------
			s[top_size] = uint8_t(carry);
            
            
            ans.rescan();
            return ans;
        }
        
        
        natural operator+( const natural &lhs, const natural &rhs )
        {
            return natural::add(lhs,rhs);
        }
        
        natural & natural:: operator+=( const natural &rhs )
        {
            natural tmp( add(*this,rhs) );
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator++()
        {
            throw;
            return *this;
        }
    }

}

