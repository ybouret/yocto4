#include "yocto/gems/atom.hpp"

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
        
    }
    
}


