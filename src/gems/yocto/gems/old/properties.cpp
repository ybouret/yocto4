#include "yocto/gems/properties.hpp"

namespace yocto
{
    namespace gems
    {

        word_t properties:: compute_uuid(const string &s) throw()
        {
            hashing::sha1 H;
            return H.key<word_t>(s);
        }

        properties:: ~properties() throw()
        {
        }

        properties:: properties(const string &id, const double m) :
        name(id),
        uuid( compute_uuid(name) ),
        mass(m),
        data()
        {
        }
        
    }
    
}

