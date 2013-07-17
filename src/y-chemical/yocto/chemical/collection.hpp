#ifndef YOCTO_CHEMICAL_COLLECTION
#define YOCTO_CHEMICAL_COLLECTION 1


#include "yocto/chemical/species.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace chemical
    {
    
#define YOCTO_CHEMICAL_NAMED_SPECIES(method,name)                   \
inline species::ptr       &method()       { return (*this)[name]; } \
inline const species::ptr &method() const { return (*this)[name]; } 
        
        class collection : public species::db
        {
        public:
            typedef map<string,int> ztable_type;
            ztable_type ztable; //!< table of predefined species

            explicit collection();
            explicit collection(size_t);
            virtual ~collection() throw();
            
            species &add( const string &name, int z);
            species &add( const char   *name, int z);
            
            bool    has( const string &name) const throw();
            bool    has( const char   *name) const;
            
            species::ptr &operator[]( const string &id );
            species::ptr &operator[]( const char   *id );
            
            const species::ptr & operator[]( const string &id ) const;
            const species::ptr & operator[]( const char   *id ) const;

            void suppress( const string &name ) throw();
            
            void update_indices() throw(); // in case a species is removed
            
            //------------------------------------------------------------------
            // species must be declared before use !
            //------------------------------------------------------------------
            YOCTO_CHEMICAL_NAMED_SPECIES(proton,      "H+")
            YOCTO_CHEMICAL_NAMED_SPECIES(hydroxy,     "HO-")
            YOCTO_CHEMICAL_NAMED_SPECIES(chloride,    "Cl-")
            YOCTO_CHEMICAL_NAMED_SPECIES(sodium,      "Na+")
            YOCTO_CHEMICAL_NAMED_SPECIES(potassium,   "K+")
            YOCTO_CHEMICAL_NAMED_SPECIES(CO2,         "CO2")
            YOCTO_CHEMICAL_NAMED_SPECIES(bicarbonate, "HCO3-")
            YOCTO_CHEMICAL_NAMED_SPECIES(carbonate,   "CO3--")
            YOCTO_CHEMICAL_NAMED_SPECIES(amonium,     "NH4+")
            YOCTO_CHEMICAL_NAMED_SPECIES(amoniac,     "NH3")
            
            species &add(const string &name); //! one of the predefined species in ztable
            species &add(const char   *name);
            
            
            void dump_ztable( std::ostream &os ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(collection);
            void build_ztable();
        };
        
    }
    
}

#endif

