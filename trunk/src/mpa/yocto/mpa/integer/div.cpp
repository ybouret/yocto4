#include "yocto/mpa/word2mpz.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpa
    {
        integer integer::div(const integer &num, const integer &den)
        {
            const natural rho = num.n/den.n;
            return integer( sign_mul(num.s,den.s), rho );
        }
        
        integer integer::div(const int64_t  num, const integer &den)
        {
            const word2mpz w(num);
            return integer::div(w.z,den);
        }
        
        integer integer:: div(const integer &num, const int64_t  den)
        {
            const word2mpz w(den);
            return integer::div(num,w.z);
        }
        
        integer operator/(const integer &lhs, const integer &rhs)
        {
            return integer::div(lhs,rhs);
        }
        
        integer operator/(const int64_t  lhs, const integer &rhs)
        {
            return integer::div(lhs,rhs);
        }
        
        
        integer operator/(const integer &lhs, const int64_t  rhs)
        {
            return integer::div(lhs,rhs);
        }
        
        integer integer:: operator/=( const integer &rhs )
        {
            integer tmp = div(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer integer:: operator*=( const int64_t  rhs )
        {
            integer tmp = div(*this,rhs);
            xch(tmp);
            return *this;
        }

        
    }

}

