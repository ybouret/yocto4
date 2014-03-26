#ifndef YOCTO_MPA_INTEGER_INCLUDED
#define YOCTO_MPA_INTEGER_INCLUDED 1

#include "yocto/mpa/natural.hpp"

namespace yocto
{
    namespace mpa
    {
      
        
        class integer : public object
        {
        public:
            enum sign_type
            {
                is_negative= -1,
                is_zero    =  0,
                is_positive=  1
            };
            
            const sign_type s; //!< sign...
            const natural   n; //!< absolute value
            integer();
            virtual ~integer() throw();
            
            integer(const integer &other);
            
            
        private:
            void check() throw();
            
        };
    }
    
    typedef mpa::integer mpz;
}

#endif

