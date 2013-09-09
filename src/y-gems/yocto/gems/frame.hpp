#ifndef YOCTO_GEMS_FRAME_INCLUDED
#define YOCTO_GEMS_FRAME_INCLUDED 1

#include "yocto/gems/library.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename T>
        class frame : public atom<T>::group
        {
        public:
            typedef typename residue<T>::ptr res_p;
            typedef typename atom<T>::ptr    atm_p;
            typedef set<word_t, res_p, key_hasher<word_t,hashing::sfh>,allocator> residue_set;
            typedef set<word_t, atm_p, key_hasher<word_t,hashing::sfh>,allocator> atom_set;
            
            virtual ~frame() throw();
            explicit frame() throw();
            
            
            
        private:
            residue_set residues_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
        };
        
    }
}

#endif
