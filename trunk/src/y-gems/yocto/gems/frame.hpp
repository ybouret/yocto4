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
            virtual ~frame() throw();
            explicit frame();
            
            
            
        private:
            typedef key_hasher<word_t,hashing::sfh>                    word_hasher;
            typedef typename atom<T>::pointer                          atom_pointer;
            typedef typename residue<T>::pointer                       residue_pointer;
            typedef  set<word_t,atom_pointer,word_hasher,allocator>    atom_set;
            typedef  set<word_t,residue_pointer,word_hasher,allocator> residue_set;
            
            atom_set    atomSet;
            residue_set residueSet;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
            
        public:
            library<T> lib;
        };
        
    }
}

#endif
