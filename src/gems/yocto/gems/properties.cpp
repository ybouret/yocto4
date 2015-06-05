#include "yocto/gems/properties.hpp"
#include "yocto/hashing/sha1.hpp"

namespace yocto
{
    namespace gems
    {


        properties:: properties(const string &id) :
        name(id),
        uuid( compute_uuid(name) )
        {
        }
        

        properties:: ~properties() throw()
        {
        }


        word_t properties:: compute_uuid(const string &id) throw()
        {
            hashing::sha1 H;
            return H.key<word_t>(id);
        }
    }
}

