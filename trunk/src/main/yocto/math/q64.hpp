#ifndef YOCTO_MATH_Q64_INCLUDED
#define YOCTO_MATH_Q64_INCLUDED 1

#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"

#include <iostream>
#include <cerrno>

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
                
                os << '(' << q.n << '/' << q.d << ')';
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
            
            
            inline q64_t operator+() const throw() { return *this; }
            
            inline q64_t & operator+=( const q64_t &rhs )
            {
                return (*this = add(*this,rhs) );
            }
            
            inline q64_t & operator+=( const num_t n )
            {
                const q64_t rhs(n);
                return (*this = add(*this,rhs) );
            }
            
            inline friend q64_t operator+(const q64_t &lhs, const q64_t &rhs)
            {
                return add(lhs,rhs);
            }
            
            inline friend q64_t operator+(const q64_t &lhs, const num_t n)
            {
                const q64_t rhs(n);
                return add(lhs,rhs);
            }
            
            inline friend q64_t operator+(const num_t n, const q64_t &rhs)
            {
                const q64_t lhs(n);
                return add(lhs,rhs);
            }
            
            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            inline static q64_t sub(const q64_t &lhs, const q64_t &rhs)
            {
                return q64_t( smul(lhs.n,rhs.d) - smul(rhs.n,lhs.d), umul(lhs.d,rhs.d) );
            }
            
            
            inline q64_t operator-() const  { return q64_t(-n,d); }
            
            inline q64_t & operator-=( const q64_t &rhs )
            {
                return (*this = sub(*this,rhs) );
            }
            
            inline q64_t & operator-=( const num_t n )
            {
                const q64_t rhs(n);
                return (*this = sub(*this,rhs) );
            }
            
            inline friend q64_t operator-(const q64_t &lhs, const q64_t &rhs)
            {
                return sub(lhs,rhs);
            }
            
            inline friend q64_t operator-(const q64_t &lhs, const num_t n)
            {
                const q64_t rhs(n);
                return sub(lhs,rhs);
            }
            
            inline friend q64_t operator-(const num_t n, const q64_t &rhs)
            {
                const q64_t lhs(n);
                return sub(lhs,rhs);
            }
            
            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            static inline q64_t mul(const q64_t &lhs, const q64_t &rhs)
            {
                return q64_t( (lhs.n*rhs.n), umul(lhs.d,rhs.d) );
            }
            
            inline q64_t & operator*=( const q64_t &rhs )
            {
                return (*this = mul(*this,rhs) );
            }
            
            inline q64_t & operator*=( const num_t n )
            {
                const q64_t rhs(n);
                return (*this = mul(*this,rhs) );
            }
            
            inline friend q64_t operator*(const q64_t &lhs, const q64_t &rhs)
            {
                return mul(lhs,rhs);
            }
            
            inline friend q64_t operator*(const q64_t &lhs, const num_t n)
            {
                const q64_t rhs(n);
                return mul(lhs,rhs);
            }
            
            inline friend q64_t operator*(const num_t n, const q64_t &rhs)
            {
                const q64_t lhs(n);
                return mul(lhs,rhs);
            }
            
            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            static inline q64_t div(const q64_t &lhs, const q64_t &rhs)
            {
                if(rhs.n>0)
                {
                    return q64_t( smul(lhs.n,rhs.d), umul(rhs.n,lhs.d) );
                }
                else
                {
                    return q64_t( smul(-lhs.n,rhs.d), umul(-rhs.n,lhs.d) );
                }
            }
            
            inline q64_t & operator/=( const q64_t &rhs )
            {
                return (*this = div(*this,rhs) );
            }
            
            inline q64_t & operator/=( const num_t n )
            {
                const q64_t rhs(n);
                return (*this = div(*this,rhs) );
            }
            
            inline friend q64_t operator/(const q64_t &lhs, const q64_t &rhs)
            {
                return div(lhs,rhs);
            }
            
            inline friend q64_t operator/(const q64_t &lhs, const num_t n)
            {
                const q64_t rhs(n);
                return div(lhs,rhs);
            }
            
            inline friend q64_t operator/(const num_t n, const q64_t &rhs)
            {
                const q64_t lhs(n);
                return div(lhs,rhs);
            }
            
            
            
            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            static inline int compare( const q64_t &lhs, const q64_t &rhs)
            {
                const num_t L = smul(lhs.n,rhs.d);
                const num_t R = smul(rhs.n,lhs.d);
                return (L<R) ? -1 : ( (R<L) ? 1 : 0 );
            }
            
#define YOCTO_Q64_COMPARE1(OP,LTYPE,RTYPE) \
inline friend bool operator OP ( const LTYPE L, const RTYPE R ) \
{ const q64_t lhs(L); const q64_t rhs(R); return compare(lhs,rhs) OP 0; }
            
#define YOCTO_Q64_COMPARE(OP) \
YOCTO_Q64_COMPARE1(OP,q64_t&, q64_t&) \
YOCTO_Q64_COMPARE1(OP,num_t,  q64_t&) \
YOCTO_Q64_COMPARE1(OP,q64_t&, num_t )
            
            YOCTO_Q64_COMPARE(==)
            YOCTO_Q64_COMPARE(!=)
            YOCTO_Q64_COMPARE(<=)
            YOCTO_Q64_COMPARE(>=)
            YOCTO_Q64_COMPARE(<)
            YOCTO_Q64_COMPARE(>)
            
        private:
            inline void check()
            {
                if(d<=0)
                    throw libc::exception( EDOM, "q64: division by zero");
                
                const den_t x = (n<0) ? den_t(-n) : den_t(n);
                const den_t g = gcd_of<den_t>(x,d);
                //std::cerr << "[n=" << n << ", d=" << d << ", x=" << x << ", g=" << g << "]" << std::endl;
                ((num_t &)n) /= num_t(g);
                ((den_t &)d) /= g;
                
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
        
        inline q64_t Fabs( q64_t q ) throw()
        {
            return q64_t( ( (q.n >= 0) ? q.n : -q.n ), q.d);
        }
    }
    
}

#include "yocto/bitwise.hpp"
YOCTO_SUPPORT_C_STYLE_OPS(math::q64_t);

#endif
