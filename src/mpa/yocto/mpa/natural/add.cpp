#include "yocto/mpa/word2mpn.hpp"
#include "yocto/code/bswap.hpp"

//#define Y_MPA_UNROLL 1

#if defined(Y_MPA_UNROLL)
#include "yocto/code/unroll.hpp"
#endif

#include <iostream>

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
            
            
#if defined(Y_MPA_UNROLL)
            //------------------------------------------------------------------
            // common loop
            //------------------------------------------------------------------
#define Y_MPA_ADD(I) carry += l[I] + r[I]; s[I] = uint8_t(carry); carry >>= 8
            YOCTO_LOOP_FUNC(com_size,Y_MPA_ADD,0);
            
            //------------------------------------------------------------------
            // top loop
            //------------------------------------------------------------------
            const size_t ntop = top_size - com_size;
            std::cerr << "ntop=" << ntop << std::endl;
            if(ntop>0)
            {
                l += com_size;
                s += com_size;
#define Y_MPA_ADD2(I) carry += l[I]; s[I] = uint8_t(carry); carry >>= 8
                std::cerr << "\tstart" << std::endl;
                YOCTO_LOOP_FUNC(ntop, Y_MPA_ADD2,0);
                std::cerr << "\tend" << std::endl;
            }
            
#else
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
                carry  += l[i];
                s[i]    = uint8_t(carry);
                carry >>= 8;
            }
#endif
            
            //------------------------------------------------------------------
            // finalize
            //------------------------------------------------------------------
            s[top_size] = uint8_t(carry);
            
            
            ans.rescan();
            YOCTO_CHECK_MPN(&ans);
            return ans;
        }
        
        
        natural natural:: add( const natural &lhs, const uint64_t x )
        {
            const word2mpn w(x);
            return add(lhs,w.n);
        }
        
        natural operator+( const natural &lhs, const natural &rhs )
        {
            return natural::add(lhs,rhs);
        }
        
        natural operator+( const natural &lhs, const uint64_t rhs)
        {
            return natural::add(lhs,rhs);
        }
        
        natural operator+( const uint64_t lhs, const natural &rhs )
        {
            return natural::add(rhs,lhs);
        }
        
        natural & natural:: operator+=( const natural &rhs )
        {
            natural tmp( add(*this,rhs) );
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator+=( const uint64_t rhs )
        {
            const word2mpn w(rhs);
            natural tmp( add(*this,w.n) );
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

