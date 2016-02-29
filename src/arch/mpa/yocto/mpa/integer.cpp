#include "yocto/mpa/integer.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/error.hpp"

#include <iostream>

namespace yocto
{
    namespace mpa
    {
        sign_type int2sign(int s) throw()
        {
            return (s<0) ? __negative : ( (0<s) ? __positive : __zero);
        }
        
        integer:: ~integer() throw()
        {
        }
        
        integer:: integer() :
        s(__zero),
        n()
        {
        }
        
        integer:: integer( const integer &other ) :
        s( other.s ),
        n( other.n )
        {
        }
        
        integer:: integer(const natural &N) :
        s( N.is_zero() ? __zero : __positive),
        n(N)
        {
        }
        
        void integer:: check() throw()
        {
            if( n.is_zero() ) (sign_type &)s = __zero;
        }
        
        
        integer::integer(const int64_t  x) :
        s( sgn_of(x) ),
        n( abs_of(x) )
        {
        }
        
        
        sign_type integer::sgn_of(const int64_t x) throw()
        {
            return x < 0 ? __negative : ( x>0 ? __positive : __zero );
        }
        
        sign_type sign_neg(const sign_type S) throw()
        {
            switch(S)
            {
                case __negative: return __positive;
                case __zero:     return __zero;
                case __positive: return __negative;
            }
            critical_error(error_invalid_data,"mpa.sign_neg: unexpected sign");
            return __zero;
        }
        
        sign_type sign_mul(const sign_type lhs,const sign_type rhs) throw()
        {
            switch(lhs)
            {
                case __negative:
                    switch(rhs) {
                        case __negative: return __positive;
                        case __zero:     return __zero;
                        case __positive: return __negative;
                    }
                    
                case __zero:
                    return __zero;
                    
                case __positive:
                    switch(rhs) {
                        case __negative: return __negative;
                        case __zero:     return __zero;
                        case __positive: return __positive;
                    }
            }
            critical_error(error_invalid_data,"mpa.sign_mul: unexpected sign");
            return __zero;
        }
        
        
        uint64_t integer::abs_of(const int64_t x) throw()
        {
            return x < 0 ? -x : x;
        }
        
        
        std::ostream & operator<<( std::ostream &os, const integer &z)
        {
            if(z.s == __negative)
                os << "-";
            os << z.n;
            return os;
        }
        
        integer:: integer( const sign_type S, const natural &N ) :
        s(S),
        n(N)
        {
            check();
        }
        
        void  integer:: xch( integer &other ) throw()
        {
            cswap_const(s,other.s);
            ((natural &)(n)).xch( (natural &)(other.n) );
        }
        
        integer & integer:: operator=( const integer &other )
        {
            integer tmp(other);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator=( const int64_t other )
        {
            integer tmp(other);
            xch(tmp);
            return *this;
        }
        
        integer & integer::operator=(const natural &N )
        {
            integer tmp(N);
            xch(tmp);
            return *this;
        }
        
        double integer:: to_double() const
        {
            double ans = n.to_double();
            return (s == __negative) ? -ans : ans;
        }

        
    }
}


