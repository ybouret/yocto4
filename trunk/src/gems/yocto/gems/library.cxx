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
        library<real_t>:: library() :
        atoms(),
        residues()
        {
            
        }
        
        template <>
        residue<real_t>:: properties &
        library<real_t>:: declare_residue(word_t type, const string &name)
        {
            if( residues.has_key(type) ) throw exception("multiple residue type 0x%08x", unsigned(type) );
            if( residues.has_sub(name) ) throw exception("multiple residue '%s'", name.c_str());
            
            residue<real_t>::properties::pointer p( new residue<real_t>::properties(type,name) );
            if(!residues.insert(p))
                throw exception("internal error for residue '%s'", name.c_str());
            
            return *p;
        }
        
        
        template <>
        atom<real_t>:: properties &
        library<real_t>:: declare_atom(word_t type, const string &name, real_t mass)
        {
            if(atoms.has_key(type) ) throw exception("multiple atom type 0x%08x", unsigned(type) );
            if(atoms.has_sub(name) ) throw exception("multiple atom '%s'", name.c_str());
           
            atom<real_t>::properties::pointer p( new atom<real_t>::properties(type,name,mass) );
            
            if( !atoms.insert(p) ) throw exception("internal error for atom '%s'", name.c_str());
                                            
            return *p;
        }
        
    }
    
}
