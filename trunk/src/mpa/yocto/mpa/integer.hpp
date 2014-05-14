#ifndef YOCTO_MPA_INTEGER_INCLUDED
#define YOCTO_MPA_INTEGER_INCLUDED 1

#include "yocto/mpa/natural.hpp"

namespace yocto
{
    namespace mpa
    {
        enum sign_type
        {
            __negative = -1,
            __zero     =  0,
            __positive =  1
        };
        
        sign_type int2sign(const int) throw();
        sign_type sign_neg(const sign_type) throw();
        sign_type sign_mul(const sign_type,const sign_type) throw();
        
        class integer : public object
        {
        public:
            
            //__________________________________________________________________
            //
            // initialisation
            //__________________________________________________________________
            const sign_type s; //!< sign...
            const natural   n; //!< absolute value
            integer();
            virtual ~integer() throw();
            integer(const integer &other);
            integer(const int64_t  x);
            integer(const natural &N);
            friend std::ostream & operator<<( std::ostream &, const integer &);
            void  xch( integer & ) throw();
            integer &operator=(const integer &);
            integer &operator=(const int64_t  );
            integer &operator=(const natural &);
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            static integer add( const integer &lhs, const integer &rhs );
            friend integer operator+(const integer &lhs, const integer &rhs);
            static integer add( const integer &lhs, const int64_t  rhs );
            friend integer operator+(const integer &lhs, const int64_t  rhs);
            friend integer operator+(const int64_t  lhs, const integer &rhs);
            integer & operator+=( const integer &rhs );
            integer & operator+=( const int64_t  rhs );
            integer & operator++();     //!< prefix
            integer   operator++ (int); //!< postfix
            friend integer operator+(const integer &);
            
            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            void   neg() throw();
            static integer sub(const integer &lhs, const integer &rhs);
            static integer sub(const int64_t  lhs, const integer &rhs);
            static integer sub(const integer &lhs, const int64_t  rhs);
            friend integer operator-(const integer &lhs,const integer &rhs);
            friend integer operator-(const int64_t  lhs, const integer &rhs);
            friend integer operator-(const integer &lhs, const int64_t  rhs);
            friend integer operator-(const integer &);
            integer & operator-=( const integer &rhs );
            integer & operator-=( const int64_t  rhs );
            integer & operator--();     //!< prefix
            integer   operator-- (int); //!< postfix
            
            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            static integer mul(const integer &lhs, const integer &rhs);
            static integer mul(const int64_t  lhs, const integer &rhs);
            friend integer operator*(const integer &lhs, const integer &rhs);
            friend integer operator*(const int64_t  lhs, const integer &rhs);
            friend integer operator*(const integer &lhs, const int64_t  rhs);
            integer operator*=( const integer &rhs );
            integer operator*=( const int64_t  rhs );
            
            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            static integer div(const integer &num, const integer &den);
            static integer div(const int64_t  num, const integer &den);
            static integer div(const integer &num, const int64_t  den);
            friend integer operator/(const integer &lhs, const integer &rhs);
            friend integer operator/(const int64_t  lhs, const integer &rhs);
            friend integer operator/(const integer &lhs, const int64_t  rhs);
            integer operator/=( const integer &rhs );
            integer operator/=( const int64_t  rhs );
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static sign_type sgn_of(const int64_t x) throw();
            static uint64_t  abs_of(const int64_t x) throw();
            
            //__________________________________________________________________
            //
            // comparison
            //__________________________________________________________________
            static int compare( const integer &lhs, const integer &rhs ) throw();
            static int compare( const integer &lhs, const int64_t  rhs ) throw();
            static int compare( const int64_t  lhs, const integer &rhs ) throw();

#define YOCTO_COMPARE_MPZ(OP) \
inline friend bool operator OP (const integer &lhs, const integer &rhs) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const integer &lhs, const int64_t  rhs) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP (const int64_t  lhs, const integer &rhs) throw() { return compare(lhs,rhs) OP 0; }
            
            YOCTO_COMPARE_MPZ(==)
            YOCTO_COMPARE_MPZ(!=)
            YOCTO_COMPARE_MPZ(<)
            YOCTO_COMPARE_MPZ(<=)
            YOCTO_COMPARE_MPZ(>)
            YOCTO_COMPARE_MPZ(>=)

            double to_double() const;
            
        private:
            void check() throw();
            integer(const sign_type S, const natural &N );
        };
    }
    
    typedef mpa::integer mpz;
}

#endif

