#ifndef YOCTO_MPA_RATIONAL_INCLUDED
#define YOCTO_MPA_RATIONAL_INCLUDED

#include "yocto/mpa/integer.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        
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

        class rational
        {
        public:
            const integer num;
            const natural den;
            
            //__________________________________________________________________
            //
            // basic API
            //__________________________________________________________________
            explicit rational();                   //!< 0/1
            virtual ~rational() throw();           //!< dtor
            rational( const rational &other );     //!< copy
            rational &operator=(const rational &); //!< assign...
            void xch(rational &other) throw();     //!< no throw exchange
            rational(const int64_t __num);
            rational(const int64_t __num,const uint64_t __den);
            rational(const integer &N, const natural &D);
            rational(const integer &N);
            rational(const natural &N);

            //__________________________________________________________________
            //
            // ADD
            //__________________________________________________________________
            static rational add( const rational &lhs, const rational &rhs);
            
            inline friend rational operator+(const rational &lhs, const rational &rhs)
            {
                return add(lhs,rhs);
            }

            
            YOCTO_MPQ_FRIENDS_LHS(+, add)
            YOCTO_MPQ_FRIENDS_RHS(+, add)

            inline rational & operator+=( const rational &rhs )
            {
                rational tmp = add(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            
            inline rational & operator+=( const integer &rhs )
            {
                const rational R(rhs);
                rational tmp = add(*this,R);
                xch(tmp);
                return *this;
            }
            
            inline rational & operator+=( const int64_t rhs )
            {
                const rational R(rhs);
                rational tmp = add(*this,R);
                xch(tmp);
                return *this;
            }

            
            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            static rational sub( const rational &lhs, const rational &rhs);
            inline friend rational operator-(const rational &lhs, const rational &rhs)
            {
                return sub(lhs,rhs);
            }
            
            YOCTO_MPQ_FRIENDS_LHS(-, sub)
            YOCTO_MPQ_FRIENDS_RHS(-, sub)
            
            inline rational & operator-=( const rational &rhs )
            {
                rational tmp = sub(*this,rhs);
                xch(tmp);
                return *this;
            }

            inline rational & operator-=( const integer &rhs )
            {
                const rational R(rhs);
                rational tmp = sub(*this,R);
                xch(tmp);
                return *this;
            }
            
            inline rational & operator-=( const int64_t rhs )
            {
                const rational R(rhs);
                rational tmp = sub(*this,R);
                xch(tmp);
                return *this;
            }
            
            
            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            static rational mul(const rational &lhs, const rational &rhs );
            inline friend rational operator*(const rational &lhs, const rational &rhs)
            {
                return mul(lhs,rhs);
            }
            
            YOCTO_MPQ_FRIENDS_LHS(*, mul)
            YOCTO_MPQ_FRIENDS_RHS(*, mul)
            
            inline  rational &operator*=(const rational &rhs)
            {
                rational tmp = mul(*this,rhs);
                xch(tmp);
                return *this;
            }
          
            inline rational & operator*=( const integer &rhs )
            {
                const rational R(rhs);
                rational tmp = mul(*this,R);
                xch(tmp);
                return *this;
            }
            
            inline rational & operator*=( const int64_t rhs )
            {
                const rational R(rhs);
                rational tmp = mul(*this,R);
                xch(tmp);
                return *this;
            }
            
            
            //! power
            static rational power( const rational &q, uint64_t p );
            
            friend std::ostream & operator<<(std::ostream &, const rational &q);
            
            
        private:
            //! automatically called
            void simplify();
        };
        
    }
    
    typedef mpa::rational mpq;
}

#endif

