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
        
#define YOCTO_MPQ_FRIENDS(OP,CALL) \
YOCTO_MPQ_FRIENDS_LHS(OP,CALL)     \
YOCTO_MPQ_FRIENDS_RHS(OP,CALL)
        
        
#define YOCTO_MPQ_COMPACT_FOR(OP,CALL,TYPE)      \
inline rational & operator OP ( const TYPE rhs ) \
{\
const rational R(rhs); \
rational tmp = CALL(*this,rhs);\
xch(tmp); \
return *this; \
}
        
#define YOCTO_MPQ_COMPACT(OP,CALL)      \
YOCTO_MPQ_COMPACT_FOR(OP,CALL,int64_t)  \
YOCTO_MPQ_COMPACT_FOR(OP,CALL,integer&) \
YOCTO_MPQ_COMPACT_FOR(OP,CALL,natural&)

        //! an always simplified rational
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
            
            
            YOCTO_MPQ_FRIENDS(+,add)
            
            inline rational & operator+=( const rational &rhs )
            {
                rational tmp = add(*this,rhs);
                xch(tmp);
                return *this;
            }
            YOCTO_MPQ_COMPACT(+=,add)
            
            rational & operator++();     //!< prefix
            rational   operator++ (int); //!< postfix
            
            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            static rational sub( const rational &lhs, const rational &rhs);
            inline friend rational operator-(const rational &lhs, const rational &rhs)
            {
                return sub(lhs,rhs);
            }
            
            YOCTO_MPQ_FRIENDS(-,sub)
            
            inline rational & operator-=( const rational &rhs )
            {
                rational tmp = sub(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            YOCTO_MPQ_COMPACT(-=,sub)
           
            friend rational operator-(const rational &q)
            {
                const integer N = -q.num;
                return rational(N,q.den);
            }

            rational & operator--();     //!< prefix
            rational   operator-- (int); //!< postfix
            
            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            static rational mul(const rational &lhs, const rational &rhs );
            inline friend rational operator*(const rational &lhs, const rational &rhs)
            {
                return mul(lhs,rhs);
            }
            
            YOCTO_MPQ_FRIENDS(*,mul)
            
            inline  rational &operator*=(const rational &rhs)
            {
                rational tmp = mul(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            YOCTO_MPQ_COMPACT(*=,mul)

            
            
            //__________________________________________________________________
            //
            // DIV
            //__________________________________________________________________
            static rational div( const rational &lhs, const rational &rhs);
            inline friend rational operator/(const rational &lhs, const rational &rhs)
            {
                return div(lhs,rhs);
            }
            YOCTO_MPQ_FRIENDS(/,div)
            
            inline  rational &operator/=(const rational &rhs)
            {
                rational tmp = div(*this,rhs);
                xch(tmp);
                return *this;
            }
            
            YOCTO_MPQ_COMPACT(/=,div)

            
            //! power
            static rational power( const rational &q, uint64_t p );
            
            //! output
            friend std::ostream & operator<<(std::ostream &, const rational &q);
            
            double to_double() const;

            //! equality, fast
            friend bool operator==(const rational &lhs, const rational &rhs) throw();

            //! inequality, fast
            friend bool operator!=(const rational &lhs, const rational &rhs) throw();

            static int compare(const rational &lhs, const rational &rhs );
            static int compare(const rational &lhs, const int64_t   rhs );
            static int compare(const int64_t   lhs, const rational &rhs );

#define YOCTO_COMPARE_MPQ(OP) \
inline friend bool operator OP (const rational &lhs, const rational &rhs) throw() { return rational::compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const rational &lhs, const int64_t   rhs) throw() { return rational::compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const int64_t   lhs, const rational &rhs) throw() { return rational::compare(lhs,rhs) OP 0; }

            YOCTO_COMPARE_MPQ(<)
            YOCTO_COMPARE_MPQ(<=)
            YOCTO_COMPARE_MPQ(>)
            YOCTO_COMPARE_MPQ(>=)
            

        private:
            //! automatically called
            void simplify();
            explicit rational( const integer &, const natural &,int2type<false>);
        };
        
    }
    
    typedef mpa::rational mpq;
    template <>
    struct xnumeric<mpq>
    {
        inline static mpq zero() { return mpn();  }
        inline static mpq one()  { return mpq(1,1); }
    };

    namespace math
    {
        inline mpa::rational Fabs( const  mpa::rational &q )
        {
             mpa::rational tmp(q);
            if(tmp.num.s==mpa::__negative)
            {
                ((mpa::integer &)(tmp.num)).neg();
            }
            return tmp;
        }
    }
}

#endif

