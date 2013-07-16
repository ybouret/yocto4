#ifndef YOCTO_CHEMICAL_SPECIES_INCLUDED
#define YOCTO_CHEMICAL_SPECIES_INCLUDED 1


#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

#include "yocto/intrusive-ptr.hpp"
#include "yocto/counted.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace chemical
    {
        
        extern const double standard_pressure;
        extern const double standard_temperature;
        
        class species : public object, public counted
        {
        public:
            const string name;
            const int    z;
            vslot        data;
            
            explicit species( const string &id, int charge);
            explicit species( const char   *id, int charge);
            virtual ~species() throw();
            
            const string &key() const throw();
            
            
            typedef intrusive_ptr<string,species> ptr;
            typedef set<string,ptr>               db;
            
            friend std::ostream & operator<<( std::ostream &, const species &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
}

#endif

