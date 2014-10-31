#ifndef YOCTO_FAME_GHOST_INCLUDED
#define YOCTO_FAME_GHOST_INCLUDED 1

#include "yocto/fame/linear.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace fame
    {
        
        class ghost : public offsets_list
        {
        public:
            explicit ghost(size_t n);
            virtual ~ghost() throw();
            
            
            //! copy data
            static void copy(linear_space       &dst,
                             const ghost        &gdst,
                             const linear_space &src,
                             const ghost        &gsrc) throw();
            
            //! save content of one ghost of a linear space
            void save(array<uint8_t> &dst, const linear_space &src) const throw();
            
            //! load some data in one ghost of a linear space
            void load(linear_space &dst, const array<uint8_t> &src) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
