#include "yocto/math/ztype.hpp"
#include "yocto/gems/library.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <>
        library<real_t>:: ~library() throw() {}
        
        template <>
        library<real_t>:: library() : atoms(), residues()
        {
        }
        
        template <>
        library<real_t>::residue_properties & library<real_t>:: declare_residue( word_t type, const string &name )
        {
            residue_properties::pointer p( new residue_properties(type,name) );
            if( !residues.insert(p) ) throw exception("multiple residue type");
            return *p;
        }
        
        
        template <>
        library<real_t>::residue_properties & library<real_t>:: declare_residue( word_t type, const char *name )
        {
            const string ID(name);
            return declare_residue(type, ID);
        }
        
        template <>
        library<real_t>::atom_properties & library<real_t>:: declare_atom( word_t type, const string &name, real_t mass)
        {
            atom_properties::pointer p( new atom_properties(type,name,mass) );
            if( !atoms.insert(p) ) throw exception("multiple atom type");
            return *p;
        }
        
        template <>
        library<real_t>::atom_properties & library<real_t>:: declare_atom( word_t type, const char *name, real_t mass)
        {
            const string ID(name);
            return declare_atom(type, ID, mass);
        }
        
        
    }
    
}
