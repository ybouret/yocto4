#ifndef YOCTO_GEMS_IDENTIFIER_INCLUDED
#define YOCTO_GEMS_IDENTIFIER_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/counted.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    namespace gems
    {
        
        class identifier : public virtual object, public counted
        {
        public:
            const word_t uuid; //!< the unique user's ID.
            const word_t type; //!< the type associated to the uuid.
            
            explicit identifier( word_t u, word_t t) throw();
            virtual ~identifier() throw();
            
            const word_t & key() const throw(); //!< uuid
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(identifier);
        };
        
    }
    
}

#endif
