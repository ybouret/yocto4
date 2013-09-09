#ifndef YOCTO_GEMS_FRAME_INCLUDED
#define YOCTO_GEMS_FRAME_INCLUDED 1

#include "yocto/gems/library.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename T>
        class frame 
        {
        public:
            typedef typename residue<T>::pointer residue_ptr;
            typedef typename atom<T>::pointer    atom_ptr;
            typedef set<word_t, residue_ptr, key_hasher<word_t,hashing::sfh>,allocator> residue_set;
            typedef set<word_t, atom_ptr, key_hasher<word_t,hashing::sfh>,allocator>    atom_set;
            
            virtual ~frame() throw();
            explicit frame();
            
            residue<T> & create( word_t type );
            
            
            
            array<atom_ptr>       &atoms() throw();
            const array<atom_ptr> &atoms() const throw();
            
            
        private:
            vector<atom_ptr,allocator> atomList_;
            residue_set                residues_;
            atom_set                   atoms_;
            word_t                     rid;
            word_t                     aid;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
        public:
            library<T> lib;
        };
        
    }
}

#endif
