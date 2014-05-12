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
        
            static rational add( const rational &lhs, const rational &rhs);
            
            void simplify();
            
        private:
            explicit rational( const integer &N, const natural &D); //simplify
        };
        
    }
}

#endif

