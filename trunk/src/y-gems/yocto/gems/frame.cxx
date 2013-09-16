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
        rid(0),
        aid(0)
        {
        }
        
        
        //======================================================================
        // adding a new residue, empty
        //======================================================================
        template <>
        word_t frame<real_t>:: add_residue(word_t type)
        {
            const residue_properties_pointer *rpp = lib.residues.search(type);
            if(!rpp)
                throw exception("no residue type in library");
            
            const residue_pointer p( new residue<real_t>(rid,type) );
            if( !residueSet.insert(p) )
                throw exception("unexpected residue '%s' insertion failure", (*rpp)->name.c_str());
            
            ++rid;
            return p->uuid;
        }
        
        //======================================================================
        // adding a new atom
        //======================================================================
        template <>
        word_t frame<real_t>:: add_atom_to( word_t residue_uuid, word_t type)
        {
            //------------------------------------------------------------------
            //-- check atom type
            //------------------------------------------------------------------
            const atom_properties_pointer *app = lib.atoms.search(type);
            if(!app)
                throw exception("no atom type in library");
            const char *name = (*app)->name.c_str();
            
            residue_pointer *parent_addr = residueSet.search( residue_uuid );
            if(!parent_addr)
                throw exception("no parent residue for atom '%s'", name);
            
            residue_pointer &parent = *parent_addr;
            
            //------------------------------------------------------------------
            //-- phase 1: insert into atomSet
            //------------------------------------------------------------------
            atom_pointer p( new atom<real_t>(parent,aid,type) );
            if(!atomSet.insert(p))
                throw exception("unexpected atom '%s' insertion in FRAME failure", name);
            
            //------------------------------------------------------------------
            //-- phase 2: add to residue
            //------------------------------------------------------------------
            try
            {
                if( !parent->insert(p) )
                    throw exception("unexpected atom '%s' insertion in RESIDUE failure",name);
            }
            catch(...)
            {
                (void) atomSet.remove(aid);
                throw;
            }
            
            ++aid;
            return p->uuid;
        }

        
        
        
    }
}
