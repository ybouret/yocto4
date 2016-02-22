#ifndef YOCTO_MPL_RATIONAL_INCLUDED
#define YOCTO_MPL_RATIONAL_INCLUDED 1

#include "yocto/mpl/integer.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpl
    {
        
        class rational : public object
        {
        public:
            const integer num;
            const natural den;
            
            virtual ~rational() throw() {}
            inline   rational() : num(0), den(1) {}
            inline   rational(const rational &r) : num(r.num), den(r.den) {}
            
            inline void xch(rational &r) throw()
            {
                ( (integer &)num ).xch( (integer &)(r.num) );
                ( (natural &)den ).xch( (natural &)(r.den) );
            }
            
            inline rational & operator=(const rational &r)
            {
                rational q(r);
                xch(q);
                return *this;
            }
            
            inline rational(const integer  &x) : num(x), den(1) {}
            inline rational(const integer_t x) : num(x), den(1) {}
            inline rational(const integer &x, const natural &y) : num(x), den(y) { update(); }
            inline rational(const integer_t x, const word_t  y) : num(x), den(y) { update(); }
            
            inline void update()
            {
                if(den.is_zero()) throw libc::exception(EINVAL,"mpq: zero denominator");
                natural::simplify( (natural &)(num.n), (natural &)(den) );
            }
            
            inline friend std::ostream & operator<<( std::ostream &os, const rational &r)
            {
                if(r.den<=1)
                {
                    os << r.num;
                }
                else
                {
                    os << '(' << r.num << '/' << r.den << ')';
                }
                
                return os;
            }
            
            //__________________________________________________________________
            //
            //
            // Addition
            //
            //__________________________________________________________________
            inline rational add( const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                const natural q = lhs.den * rhs.den;
                const integer w = u+v;
                return rational(w,q);
            }
            
            //__________________________________________________________________
            //
            //
            // Subtraction
            //
            //__________________________________________________________________
            inline rational sub( const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                const natural q = lhs.den * rhs.den;
                const integer w = u-v;
                return rational(w,q);
            }
            
            //__________________________________________________________________
            //
            //
            // multiplication
            //
            //__________________________________________________________________
            inline rational mul(const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.num;
                const natural v = lhs.den * rhs.den;
                return rational(u,v);
            }
            
            //__________________________________________________________________
            //
            //
            // division
            //
            //__________________________________________________________________
            inline rational div(const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                (sign_type &)(u.s) = sign_mul(u.s,v.s);
                return rational(u,v.n);
            }
            
        };
    }
    
    typedef mpl::rational mpq;
    template <>
    struct xnumeric<mpq>
    {
        inline static mpq zero() { return mpq();  }
        inline static mpq one()  { return mpq(1); }
    };
}

#endif

