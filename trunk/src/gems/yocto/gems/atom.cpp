#include "yocto/gems/atom.hpp"

#include <iostream>

namespace yocto
{
    namespace gems
    {

        atom_info:: ~atom_info() throw() {}

        const word_t & atom_info:: key() const throw() { return uuid; }

        atom_info:: atom_info(const word_t u, const properties &p ) throw() :
        uuid(u),
        ppty(p)
        {}

        bool atom_info::is_orphan(const handle &p) throw()
        {
            const bool ans = (1==p->refcount());
            if(ans)
            {
                std::cerr << "atom$" << p->uuid << " is orphan..." <<std::endl;
            }
            return ans;
        }

    }

}


