#ifndef YOCTO_GEMS_FRAME_INCLUDED
#define YOCTO_GEMS_FRAME_INCLUDED 1

#include "yocto/gems/library.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename T>
        class frame 
        {
        public:
            typedef key_hasher<word_t,hashing::sfh>                    word_hasher;
            typedef typename residue<T>::properties::pointer           residue_properties_pointer;
            typedef typename atom<T>::properties::pointer              atom_properties_pointer;
            typedef typename atom<T>::pointer                          atom_pointer;
            typedef typename residue<T>::pointer                       residue_pointer;
            typedef  set<word_t,atom_pointer,word_hasher,allocator>    atom_set;
            typedef  set<word_t,residue_pointer,word_hasher,allocator> residue_set;
            typedef  typename atom<T>::group                           atom_list;
            
            virtual ~frame() throw();
            explicit frame();
            
            //! new residue
            /**
             \return the residue uuid
             */
            word_t add_residue( const word_t  type);
            word_t add_residue( const string &name);
            
            //! new atom
            /**
             \return the atom uuid
             */
            word_t add_atom_to( word_t residue_uuid, word_t type);
            word_t add_atom_to( word_t residue_uuid, const string &name);

            
            
        private:
            atom_list      atomList;
            atom_set       atomSet;
            residue_set    residueSet;

            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
            
        public:
            library<T> lib;
            
        private:
            word_t rid;
            word_t aid;
        };
        
    }
}

#endif
