#ifndef YOCTO_CHEMICAL_SPECIES_INCLUDED
#define YOCTO_CHEMICAL_SPECIES_INCLUDED 1

#include "yocto/ptr/arc.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace chemical
    {

#if 0
        //! a species
        class species : public counted_object, public vslot
        {
        public:
            typedef arc_ptr<species> pointer;


            const string name;
            const int    z;
            
            explicit species(const string &sp_name, const int sp_z);
            virtual ~species() throw();

            const string key()  throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
#endif
        
    }
}

#endif
