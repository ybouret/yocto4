#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"


namespace yocto
{
    namespace chemical
    {
        
        class equilibria : public equilibrium::db
        {
        public:
            virtual ~equilibria() throw();
            explicit equilibria();
          
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            
        };
        
        
    }
}

#endif

