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
            typedef intr_ptr<string,species> pointer;

            const string name;    //!< full name
            const int    charge;  //!< algebraic charge
            const groups formula; //!< molecular formula


            virtual ~species() throw();

            static string   nameOf(const xnode *molecule);
            static species *create(const xnode *molecule);

        private:
            explicit species(const string &sp_name, const int sp_charge, const groups &sp_formula);

            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };

    }
}

#endif
