#ifndef YOCTO_MPA_RATIONAL_INCLUDED
#define YOCTO_MPA_RATIONAL_INCLUDED

#include "yocto/mpa/integer.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        class rational
        {
        public:
            const integer num;
            const natural den;
            
            //
            explicit rational();
            virtual ~rational() throw();
            rational( const rational &other );
            rational &operator=(const rational &);
            void xch(rational &other) throw();
            rational(const int64_t __num);
            rational(const int64_t __num,const uint64_t __den);
            rational( const integer &N, const natural &D); //simplify

            // ADD
            static rational add( const rational &lhs, const rational &rhs);
            inline friend rational operator+(const rational &lhs, const rational &rhs)
            {
                return add(lhs,rhs);
            }
            inline friend rational operator+(const rational &lhs, const int64_t rhs)
            {
                const rational R(rhs);
                return add(lhs,R);
            }
            inline friend rational operator+(const int64_t lhs, const rational &rhs)
            {
                const rational L(lhs);
                return add(L,rhs);
            }
            
            inline rational & operator+=( const rational &rhs )
            {
                rational tmp = add(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            inline rational & operator+=( const int64_t rhs )
            {
                rational tmp = add(*this,rhs);
                xch(tmp);
                return *this;
            }

            
            
            // SUB
            static rational sub( const rational &lhs, const rational &rhs);
            inline friend rational operator-(const rational &lhs, const rational &rhs)
            {
                return sub(lhs,rhs);
            }
            
            inline rational & operator-=( const rational &rhs )
            {
                rational tmp = sub(*this,rhs);
                xch(tmp);
                return *this;
            }

            
            
            // MUL
            static rational mul(const rational &lhs, const rational &rhs );
            inline friend rational operator*(const rational &lhs, const rational &rhs)
            {
                return mul(lhs,rhs);
            }
            
            inline  rational &operator*=(const rational &rhs)
            {
                rational tmp = mul(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            // algebra
            void simplify();
            
            // binomial coefficients
            static rational binomial( uint64_t n, uint64_t p );
            
            // power
            static rational power( const rational &q, uint64_t p );
            
            friend std::ostream & operator<<(std::ostream &, const rational &q);
            
            
        private:
        };
        
    }
    
    typedef mpa::rational mpq;
}

#endif

