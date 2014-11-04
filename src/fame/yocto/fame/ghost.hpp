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
            
            static size_t chunk_size_of( const array<linear_handle> &handles ) throw();
            
            //! copy data
            static void copy(linear_space       &dst,
                             const ghost        &gdst,
                             const linear_space &src,
                             const ghost        &gsrc) throw();
            
            
            size_t save(void *dst, const size_t num, const array<linear_handle> &handles ) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
