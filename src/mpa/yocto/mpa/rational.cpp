#include "yocto/mpa/rational.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include <iostream>

namespace yocto
{
    
    namespace mpa
    {
        
        
        rational:: rational() :
        num(  int64_t(0) ),
        den( uint64_t(1) )
        {
        }
        
        rational:: rational( const integer &N, const natural &D) :
        num(N),
        den(D)
        {
            simplify();
        }
        
        void rational::simplify()
        {
            if(den<=0)
                throw libc::exception(EDOM,"rational division by zero");
            natural::simplify( (natural&)(num.n),(natural &)den);
        }
        
        rational:: rational( const int64_t __num ) :
        num(__num),
        den(1)
        {
        }
        
        rational:: rational( const  int64_t __num, const uint64_t __den ) :
        num(__num),
        den(__den)
        {
            simplify();
        }
        
        
        
        rational:: ~rational() throw()
        {
        }
        
        rational:: rational( const rational &other ) :
        num( other.num ),
        den( other.den )
        {
        }
        
        void rational:: xch( rational &other ) throw()
        {
            ((integer &)num).xch( (integer &)(other.num));
            ((natural &)den).xch( (natural &)(other.den));
        }
        
        rational & rational:: operator=(const rational &q )
        {
            rational tmp(q);
            xch(tmp);
            return *this;
        }
        
        //______________________________________________________________________
        //
        // ADD
        //______________________________________________________________________
        rational rational::add( const rational &lhs, const rational &rhs )
        {
            const integer LD(lhs.den);
            const integer RD(rhs.den);
            const integer N = (lhs.num*RD) + (rhs.num*LD);
            const natural D = rhs.den * lhs.den;
            return rational(N,D);
        }
        
        //______________________________________________________________________
        //
        // SUB
        //______________________________________________________________________
        rational rational::sub( const rational &lhs, const rational &rhs )
        {
            const integer LD(lhs.den);
            const integer RD(rhs.den);
            const integer N = (lhs.num*RD) - (rhs.num*LD);
            const natural D = rhs.den * lhs.den;
            return rational(N,D);
        }

        //______________________________________________________________________
        //
        // MUL
        //______________________________________________________________________
        rational rational:: mul(const rational &lhs, const rational &rhs )
        {

            const integer N = lhs.num * rhs.num;
            const natural D = lhs.den * rhs.den;
            return rational(N,D);
        }

        rational rational:: binomial( uint64_t n, uint64_t p )
        {
            if(p>n) throw libc::exception( EDOM, "invalid binomial coefficients");
            natural NN = natural::factorial(n);
            natural D  = natural::factorial(p);
            natural::simplify(NN, D);
            D  *= natural::factorial(n-p);
            const integer N(NN);
            return rational(N,D);
        }
        
        rational rational:: power( const rational &q, uint64_t p )
        {
            integer N = 1;
            natural D = 1;
            while(p-->0)
            {
                N *= q.num;
                D *= q.den;
            }
            return rational(N,D);
        }
        
        std::ostream & operator<<(std::ostream &os, const rational &q)
        {
            os << '(' << q.num << '/' << q.den << ')';
            return os;
        }
        
    }
    
}
