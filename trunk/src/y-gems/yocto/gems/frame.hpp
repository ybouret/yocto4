#ifndef YOCTO_GEMS_FRAME_INCLUDED
#define YOCTO_GEMS_FRAME_INCLUDED 1

#include "yocto/gems/residue.hpp"
#include "yocto/gems/type-table.hpp"
#include "yocto/associative/set.hpp"

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
            typedef set<word_t, res_p, key_hasher<word_t,hashing::sfh>,allocator> res_set;
            typedef set<word_t, atm_p, key_hasher<word_t,hashing::sfh>,allocator> atm_set;
            
            virtual ~frame() throw();
            explicit frame() throw();
            
            type_table reslib;
            type_table atmlib;
            
            
            
        private:
            
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
        };
        
    }
}

#endif
