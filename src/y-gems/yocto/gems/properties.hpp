#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/counted.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gems
    {
        
        class properties : public object, public counted, public vslot
        {
        public:
            const word_t type;
            const string name;
            
            explicit properties(word_t t, const string &n);
            virtual ~properties() throw();
            
            const word_t &key() const throw(); //!< this->type
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };
     
        
        
    }
    
}



#endif
