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
        
        rational:: rational( const integer & __num ) :
        num(__num),
        den(1)
        {
        }
        
        rational:: rational( const natural & __num ) :
        num(__num),
        den(1)
        {
        }
        
        rational:: rational( const integer &N, const natural &D,int2type<false>):
        num(N),
        den(D)
        {
            // do not simply nor check
            assert(D>0);
            assert(natural::are_coprime(num.n,den));
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
        
        rational & rational::operator++()
        {
            const rational __one__(1);
            rational tmp = add(*this,__one__);
            xch(tmp);
            return *this;
        }
        
        rational  rational:: operator++ (int)
        {
            const rational save(*this);
            
            const rational __one__(1);
            rational tmp = add(*this,__one__);
            xch(tmp);
            
            return save;
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
        
        rational & rational::operator--()
        {
            const rational __one__(1);
            rational tmp = sub(*this,__one__);
            xch(tmp);
            return *this;
        }
        
        rational  rational:: operator-- (int)
        {
            const rational save(*this);
            
            const rational __one__(1);
            rational tmp = sub(*this,__one__);
            xch(tmp);
            
            return save;
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
        
        rational rational:: power( const rational &q, uint64_t p )
        {
            integer N = 1;
            natural D = 1;
            while(p-->0)
            {
                N *= q.num;
                D *= q.den;
            }
            return rational(N,D,int2type<false>());
        }
        
        std::ostream & operator<<(std::ostream &os, const rational &q)
        {
            os << '(' << q.num << '/' << q.den << ')';
            return os;
        }
        
        
        //______________________________________________________________________
        //
        // DIV
        //______________________________________________________________________
        rational rational:: div(const rational &lhs, const rational &rhs)
        {
            if(rhs.num.s == __zero)
                throw libc::exception( EDOM, "rational division by zero");
            const integer Rden = rhs.den;
            const integer Lden = lhs.den;
            integer iN   = lhs.num * Rden;
            integer iD   = rhs.num * Lden;
            if(sign_mul(iN.s, iD.s) == __negative )
                iN.neg();
            
            return rational(iN,iD.n);
        }
        
        // conversion
        double rational:: to_double() const
        {
            double ans = 0;
            if(num.s==__zero) return ans;
            
            natural       n = num.n;
            // integer part
            natural       q = n/den;
            if(q>0)
            {
                ans = q.to_double();
            }
            
            // keep fractional part
            n -= q * den;
            assert(n<den);
            const natural ten = 10;
            double factor     = 0.1;
            for(size_t i=0;i<15;++i)
            {
                n *= ten;
                q  = n/den;
                ans += q.to_double() * factor;
                factor *= 0.1;
                n -= q*den;
            }
            return num.s == __negative ? -ans : ans;
        }
        
        
    }
    
    
}
