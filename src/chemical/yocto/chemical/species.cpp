#include "yocto/chemical/species.hpp"

namespace yocto
{
    namespace  chemical
    {
        species:: ~species() throw()
        {
        }

        species:: species(const string &sp_name, const int sp_z) :
        counted_object(),
        vslot(),
        name(sp_name),
        z(sp_z)
        {
        }

        
    }
}