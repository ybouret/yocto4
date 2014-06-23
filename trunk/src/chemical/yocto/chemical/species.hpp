#ifndef YOCTO_CHEMICAL_SPECIES_INCLUDED
#define YOCTO_CHEMICAL_SPECIES_INCLUDED 1


#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/associative/lexicon.hpp"

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace chemical
    {
        
        extern const double standard_pressure;
        extern const double standard_temperature;
        
        //! a species
        class species : public object, public counted
        {
        public:
            const string name; //!< unique name
            const int    z;    //!< algebraic charge
            vslot        data; //!< whatever, see vslot
            size_t       indx; //!< index in collection
            
            explicit species( const string &id, int charge);
            explicit species( const char   *id, int charge);
            virtual ~species() throw();
            
            const string &key() const throw(); //!< the name
            
            
            typedef intr_ptr<string,species> ptr; //!< smart pointer
            //typedef set<string,ptr>          db;  //!< database of smart pointers
            typedef lexicon<string,ptr>      db;  //!< database of smart pointers

            friend std::ostream & operator<<( std::ostream &, const species &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
}

#endif

