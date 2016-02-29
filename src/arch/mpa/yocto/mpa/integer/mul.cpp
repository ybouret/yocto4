#include "yocto/mpa/word2mpz.hpp"
#include "yocto/error.hpp"

namespace yocto
{
    namespace mpa
    {
        integer integer:: mul( const integer &lhs, const integer &rhs )
        {
            switch( sign_mul(lhs.s,rhs.s) )
            {
                case __negative: { const natural p = lhs.n * rhs.n; return integer( __negative, p); }
                case __zero: return integer();
                case __positive: { const natural p = lhs.n * rhs.n; return integer( __positive, p); }
            }
	    critical_error(error_invalid_data,"integer.mul invalid sign");
	    return integer();
        }
        
        integer integer:: mul(const int64_t  lhs, const integer &rhs)
        {
            const word2mpz w(lhs);
            return   mul(w.z,rhs);
        }
        
        integer operator*(const integer &lhs, const integer &rhs)
        {
            return integer::mul(lhs,rhs);
        }
        
        integer operator*(const int64_t  lhs, const integer &rhs)
        {
            return integer::mul(lhs,rhs);
        }
        
        
        integer operator*(const integer &lhs, const int64_t  rhs)
        {
            return integer::mul(rhs,lhs);
        }
        
        integer integer:: operator*=( const integer &rhs )
        {
            integer tmp = mul(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer integer:: operator*=( const int64_t  rhs )
        {
            integer tmp = mul(rhs,*this);
            xch(tmp);
            return *this;
        }

        
    }
    
}
