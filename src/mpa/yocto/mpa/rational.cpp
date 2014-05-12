#include "yocto/mpa/rational.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        
        rational:: rational() :
        num(0),
        den(1)
        {
        }
        
        rational:: rational( const integer &N, const natural &D) :
        num(N),
        den(D)
        {
            
        }
        
        void rational::simplify()
        {
            assert(den>0);
            natural::simplify( (natural&)(num.n),(natural &)den);
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
        
        
    }
    
}
