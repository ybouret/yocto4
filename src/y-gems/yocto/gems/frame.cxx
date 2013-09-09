#include "yocto/math/ztype.hpp"
#include "yocto/gems/frame.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        template <>
        frame<real_t>:: ~frame() throw()
        {
        }
        
        template <>
        frame<real_t>:: frame() :
        atomList_(),
        residues_(),
        atoms_(),
        rid(1),
        aid(1),
        lib()
        {
        }
        
        
        template <>
        residue<real_t> & frame<real_t>:: add_residue( word_t type )
        {
            //-- check exists
            if( lib.residues.search(type) == 0 )
            {
                throw exception("undeclared residue type");
            }
            
            //-- instanciate
            residue_ptr p( new residue<real_t>(rid,type) );
            
            //-- record
            if( !residues_.insert(p) )
                throw exception("unexpected multiple residues uuid");
            
            
            ++rid;
            return *p;
        }
        
    }
}
