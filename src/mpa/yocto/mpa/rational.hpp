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
            rational(const integer &N, const natural &D); //simplify
            rational(const integer &N);
            rational(const natural &N);

            // ADD
            static rational add( const rational &lhs, const rational &rhs);
            inline friend rational operator+(const rational &lhs, const rational &rhs)
            {
                return add(lhs,rhs);
            }
            
#define YOCTO_MPQ_FRIEND_LHS(OP,CALL,TYPE) \
inline friend rational operator OP ( const TYPE lhs, const rational &rhs ) \
{ const rational L(lhs); return CALL(L,rhs); }
           
#define YOCTO_MPQ_FRIENDS_LHS(OP,CALL) \
YOCTO_MPQ_FRIEND_LHS(OP,CALL,int64_t)  \
YOCTO_MPQ_FRIEND_LHS(OP,CALL,integer&) \
YOCTO_MPQ_FRIEND_LHS(OP,CALL,natural&)
            
#define YOCTO_MPQ_FRIEND_RHS(OP,CALL,TYPE) \
inline friend rational operator OP ( const rational &lhs, const TYPE rhs ) \
{ const rational R(rhs); return CALL(lhs,R); }
            
#define YOCTO_MPQ_FRIENDS_RHS(OP,CALL) \
YOCTO_MPQ_FRIEND_RHS(OP,CALL,int64_t)  \
YOCTO_MPQ_FRIEND_RHS(OP,CALL,integer&) \
YOCTO_MPQ_FRIEND_RHS(OP,CALL,natural&)

            
            YOCTO_MPQ_FRIENDS_LHS(+, add)
            YOCTO_MPQ_FRIENDS_RHS(+, add)

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

