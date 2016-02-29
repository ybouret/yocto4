#include "yocto/mpa/word2mpn.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto {
    
    namespace mpa
    {
        typedef natural::bword_t bword;
        
        
        natural natural:: bitwise( const natural &lhs, const natural &rhs, bproc ops )
        {
            assert(ops);
            size_t          nmax = lhs.size;
            const  uint8_t *umax = lhs.byte;
            size_t          nmin = rhs.size;
            const  uint8_t *umin = rhs.byte;
            if(nmax<nmin)
            {
                cswap(nmax,nmin);
                cswap(umax,umin);
            }
            
            natural ans(nmax,as_capacity);
            ans.size   = nmax;
            uint8_t *p = ans.byte;
            for(size_t i=0;i<nmin;++i)     p[i] = uint8_t( ops(umin[i],umax[i]) );
            for(size_t i=nmin;i<nmax;++i)  p[i] = umax[i];
            
            ans.update();
            return ans;
        }
        
        //______________________________________________________________________
        //
        // AND
        //______________________________________________________________________

        static inline bword bw_and( bword a, bword b ) throw()
        {
            //return (a and b);
			return a&b;
        }
        
        natural natural:: __and(const natural &lhs, const natural &rhs )
        {
            return bitwise(lhs, rhs, bw_and);
        }
        
        natural natural:: __and(const natural &lhs, const uint64_t rhs )
        {
            const word2mpn w(rhs);
            return bitwise(lhs,w.n,bw_and);
        }
        
        natural operator&(const natural &lhs, const natural &rhs)
        {
            return natural::__and(lhs,rhs);
        }
        
        natural operator&(const natural &lhs, const uint64_t rhs)
        {
            return natural:: __and(lhs, rhs);
        }
        
        natural operator&(const uint64_t lhs, const natural &rhs)
        {
            return natural::__and(rhs,lhs);
        }
        
        natural & natural:: operator&=(const natural &rhs)
        {
            natural tmp = __and(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator&=(const uint64_t rhs)
        {
            natural tmp = __and(*this,rhs);
            xch(tmp);
            return *this;
        }

        //______________________________________________________________________
        //
        // OR
        //______________________________________________________________________
        static inline bword bw_or( bword a, bword b ) throw()
        {
            //return (a or b);
			return a|b;
        }
        
        natural natural:: __or(const natural &lhs, const natural &rhs )
        {
            return bitwise(lhs, rhs, bw_or);
        }
        
        natural natural:: __or(const natural &lhs, const uint64_t rhs )
        {
            const word2mpn w(rhs);
            return bitwise(lhs, w.n, bw_or);
        }

        natural operator|(const natural &lhs, const natural &rhs)
        {
            return natural::__or(lhs,rhs);
        }
        
        natural operator|(const natural &lhs, const uint64_t rhs)
        {
            return natural:: __or(lhs, rhs);
        }
        
        natural operator|(const uint64_t lhs, const natural &rhs)
        {
            return natural::__or(rhs,lhs);
        }
        
        natural & natural:: operator|=(const natural &rhs)
        {
            natural tmp = __or(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator|=(const uint64_t rhs)
        {
            natural tmp = __or(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        //______________________________________________________________________
        //
        // XOR
        //______________________________________________________________________
        static inline bword bw_xor( bword a, bword b ) throw()
        {
            //return (a xor b);
			return a^b;
        }
        
        natural natural:: __xor(const natural &lhs, const natural &rhs )
        {
            return bitwise(lhs, rhs, bw_xor);
        }
        
        natural natural:: __xor(const natural &lhs, const uint64_t rhs )
        {
            const word2mpn w(rhs);
            return bitwise(lhs, w.n, bw_xor);
        }
        
        natural operator^(const natural &lhs, const natural &rhs)
        {
            return natural::__xor(lhs,rhs);
        }
        
        natural operator^(const natural &lhs, const uint64_t rhs)
        {
            return natural:: __xor(lhs, rhs);
        }
        
        natural operator^(const uint64_t lhs, const natural &rhs)
        {
            return natural::__xor(rhs,lhs);
        }
        
        natural & natural:: operator^=(const natural &rhs)
        {
            natural tmp = __xor(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator^=(const uint64_t rhs)
        {
            natural tmp = __xor(*this,rhs);
            xch(tmp);
            return *this;
        }

        
    }
}
