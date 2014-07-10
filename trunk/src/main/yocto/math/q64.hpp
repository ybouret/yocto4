#ifndef YOCTO_MATH_Q64_INCLUDED
#define YOCTO_MATH_Q64_INCLUDED 1

#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"

#include <iostream>

namespace yocto
{
    
    namespace math
    {
        
        class q64_t
        {
        public:
            typedef int64_t  num_t;
            typedef uint64_t den_t;
            
            const num_t n;
            const den_t d;
            
            //__________________________________________________________________
            //
            // basic API
            //__________________________________________________________________

            inline  q64_t() throw() : n(0), d(1) {}
            inline  q64_t(const q64_t &q) throw() : n(q.n), d(q.d) {}
            inline ~q64_t() throw() {}
            inline  q64_t & operator=(const q64_t &q) throw()
            {
                (num_t &)n = q.n;
                (den_t &)d = q.d;
                return *this;
            }
            
            inline q64_t( num_t N ) throw() : n(N), d(1) {}
            inline q64_t( num_t N, den_t D) : n(N), d(D) { check(); }
            
            inline q64_t & operator=( const num_t N ) throw()
            {
                (num_t &)n = N;
                (den_t &)d = 1;
                return *this;
            }
            
            inline friend std::ostream & operator<<( std::ostream &os, const q64_t &q )
            {
                os << '(' << q.n << ',' << q.d << ')';
                return os;
            }
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            inline static q64_t add(const q64_t &lhs, const q64_t &rhs)
            {
                return q64_t( smul(lhs.n,rhs.d) + smul(rhs.n,lhs.d), umul(lhs.d,rhs.d) );
            }
            
            inline q64_t & operator+=( const q64_t &rhs )
            {
                return (*this = add(*this,rhs) );
            }
            
            
            
            
        private:
            inline void check()
            {
                if(d<=0)
                    throw libc::exception( EDOM, "q64: division by zero");
                const den_t x = (n<0) ? -n : n;
                const den_t g = gcd_of(x,d);
                (num_t &)n /= g;
                (den_t &)d /= g;
                
            }
            
            inline static num_t smul( num_t x, den_t y)
            {
                return x*y;
            }
            
            inline static den_t umul(den_t x, den_t y)
            {
                return x*y;
            }
            
        };
        
    }
    
}

#endif
