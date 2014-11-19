#ifndef YOCTO_CHEMICAL_SPECIES_INCLUDED
#define YOCTO_CHEMICAL_SPECIES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/counted-object.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        //! a chemical species
        /**
         all specific data (but the charge)
         are held in a vslot structutre
         */
        class species : public counted_object
        {
        public:
            explicit species(const string &id,
                             const int     charge);
            virtual ~species() throw();
            
            const string    name; //!< unique name
            const int       z;    //!< algebraic charge
            const size_t    indx; //!< index in collection
            vslot           data; //!< whatever...
            
            const string & key() const throw();
            
            typedef intr_ptr<string,species> pointer;
            typedef set<string,pointer>      database;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
}

#endif
