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
            
#define YOCTO_MPQ_DECL(OP,OP1,CALL) \
inline friend rational operator OP ( const rational &lhs, const rational &rhs ) \
{ return CALL(lhs,rhs); }\
inline  rational & operator OP1(const rational &rhs) { rational r = *this OP rhs; xch(r); return *this; }
            
            //__________________________________________________________________
            //
            //
            // Addition
            //
            //__________________________________________________________________
            inline static rational add( const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                const natural q = lhs.den * rhs.den;
                const integer w = u+v;
                return rational(w,q);
            }
            inline rational operator+() const { return *this; }
            YOCTO_MPQ_DECL(+,+=,add)
            
            //__________________________________________________________________
            //
            //
            // Subtraction
            //
            //__________________________________________________________________
            inline static rational sub( const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                const natural q = lhs.den * rhs.den;
                const integer w = u-v;
                return rational(w,q);
            }
            
            inline rational operator-() const { const integer nn = -num; return rational(nn,den); }
            YOCTO_MPQ_DECL(-,-=,sub)

            //__________________________________________________________________
            //
            //
            // multiplication
            //
            //__________________________________________________________________
            inline static rational mul(const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.num;
                const natural v = lhs.den * rhs.den;
                return rational(u,v);
            }
            YOCTO_MPQ_DECL(*,*=,mul)

            //__________________________________________________________________
            //
            //
            // division
            //
            //__________________________________________________________________
            inline static rational div(const rational &lhs, const rational &rhs )
            {
                const integer u = lhs.num * rhs.den;
                const integer v = rhs.num * lhs.den;
                (sign_type &)(u.s) = sign_mul(u.s,v.s);
                return rational(u,v.n);
            }
            YOCTO_MPQ_DECL(/,/=,div)

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

