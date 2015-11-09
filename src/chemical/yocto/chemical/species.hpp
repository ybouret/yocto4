#ifndef YOCTO_CHEMICAL_SPECIES_INCLUDED
#define YOCTO_CHEMICAL_SPECIES_INCLUDED 1

#include "yocto/chemical/group.hpp"
#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace chemical
    {

        typedef lang::syntax::xnode xnode;

        class species : public counted_object
        {
        public:
            const string name;
            const int    charge;
            const groups formula;


            virtual ~species() throw();

            static string name_of( const xnode *molecule );

        private:
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };

    }
}

#endif
