#include "yocto/chemical/species.hpp"
#include <iostream>

namespace yocto
{
    namespace chemical
    {
        
        const double standard_pressure    = 1.0;
        const double standard_temperature = 273.15 + 20;
        
        const string & species:: key() const throw() { return name; }
        
        species:: ~species() throw() {}
        
        species:: species(const string &id, int charge ) :
        name(id),
        z(charge),
        data(),
        indx(0)
        {
        }
        
        species:: species(const char *id, int charge ) :
        name(id),
        z(charge),
        data(),
        indx(0)
        {
        }

        std::ostream & operator<<( std::ostream &os, const species &sp)
        {
            os << sp.name << '/' << sp.z;
            return os;
        }
        
    
        
    }
    
}
