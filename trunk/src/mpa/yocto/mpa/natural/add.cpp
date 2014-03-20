#include "yocto/mpa/word2mpn.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace mpa
    {
        natural natural:: add( const natural &lhs, const natural &rhs )
        {
            YOCTO_CHECK_MPN(&lhs);
            YOCTO_CHECK_MPN(&rhs);
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
            YOCTO_CHECK_MPN(&ans);
            return ans;
        }
        
        
        natural natural:: add( const natural &lhs, uint64_t x )
        {
            const word2mpn w(x);
            return add(lhs,w.n);
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
            inc(1);
            return *this;
        }
        
        natural natural:: operator++ (int)
        {
            const natural saved( *this );
            inc(1);
            return saved;
        }
        
        void natural:: inc( uint8_t b )
        {
            if(b)
            {
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(natural) ];
                natural *rhs = (natural *)&wksp[0];
                rhs->maxi    = 1;
                rhs->size    = 1;
                rhs->byte    = &b;
                natural sum( add(*this,*rhs) );
                xch(sum);
            }
        }
        
    }
    
}

