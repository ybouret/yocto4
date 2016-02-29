#include "yocto/mpa/word2mpz.hpp"

namespace yocto
{
    
    namespace mpa
    {
        void integer:: neg() throw()
        {
            (sign_type&)s = sign_neg(s);
        }

        integer integer:: sub(const integer &lhs, const integer &rhs)
        {
            ( (integer &)rhs ).neg();
            try
            {
                const integer ans = add(lhs,rhs);
                ( (integer &)rhs ).neg();
                return ans;
            }
            catch(...)
            {
                ( (integer &)rhs ).neg();
                throw;
            }
        }
        
        integer integer:: sub(const int64_t  lhs, const integer &rhs)
        {
            const word2mpz w(lhs);
            return sub(w.z,rhs);
        }
        
        integer integer:: sub(const integer &lhs, const int64_t  rhs)
        {
            const word2mpz w(rhs);
            return sub(lhs,w.z);
        }
        
        integer operator-(const integer &lhs, const integer &rhs ) { return integer::sub(lhs,rhs); }
        integer operator-(const int64_t  lhs, const integer &rhs) { return integer::sub(lhs,rhs); }
        integer operator-(const integer &lhs, const int64_t  rhs) { return integer::sub(lhs,rhs); }
        integer operator-(const integer &x)
        {
            return integer( sign_neg(x.s), x.n );
        }
        
        
        integer & integer:: operator-=( const integer &rhs )
        {
            integer tmp = sub(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator-=( const int64_t rhs )
        {
            integer tmp = sub(*this,rhs);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator--()
        {
            static const word2mpz minus_one(-1);
            integer tmp = add(*this,minus_one.z);
            xch(tmp);
            return *this;
        }
        
        integer integer:: operator--(int)
        {
            static const word2mpz minus_one(-1);
            const integer save(*this);
            integer tmp = add(*this,minus_one.z);
            xch(tmp);
            return save;
        }

        
    }
    
}
