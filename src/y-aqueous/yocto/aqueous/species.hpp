#ifndef YOCTO_AQ_SPECIES_INCLUDED
#define YOCTO_AQ_SPECIES_INCLUDED 1

#include "yocto/intrusive-ptr.hpp"
#include "yocto/string.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        class species : public object, public counted
        {
        public:
            
            explicit species(const string &id,
                             const int    charge,
                             size_t       extra_bytes         = 0,
                             void       (*extra_clean)(void*) = 0
                             );
            
            virtual ~species() throw();
            
            const string name; //!< unique name
            const int    z;    //!< algebraic charge
            
            
            typedef intrusive_ptr<string,species> ptr;
            const string & key() const throw();
            
        private:
            size_t blen;
            void  *addr;
            void (*kill)(void *);
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
    
}

#endif
