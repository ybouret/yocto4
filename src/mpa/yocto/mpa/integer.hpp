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
        
        sign_type int2sign(int) throw();
        
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
            friend std::ostream & operator<<( std::ostream &, const integer &);
            void  xch( integer & ) throw();
            integer &operator=(const integer &);
            integer &operator=(const int64_t  );
            
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
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static sign_type sgn_of(const int64_t x) throw();
            static uint64_t  abs_of(const int64_t x) throw();
            static sign_type neg_of(const sign_type) throw();
            
        private:
            void check() throw();
            integer(const sign_type S, const natural &N );
        };
    }
    
    typedef mpa::integer mpz;
}

#endif

