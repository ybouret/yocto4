#ifndef YOCTO_FAME_GHOST_INCLUDED
#define YOCTO_FAME_GHOST_INCLUDED 1

#include "yocto/fame/linear-handles.hpp"

namespace yocto
{
    namespace fame
    {
        
        
        class ghost : public offsets_list
        {
        public:
            explicit ghost(size_t n);
            virtual ~ghost() throw();
            
            
            //! copy local data
            static void copy(linear_space       &dst,
                             const ghost        &gdst,
                             const linear_space &src,
                             const ghost        &gsrc) throw();
            
            
            
            //! save interleaved data into an array of bytes
            /**
             \param  dst the adress of the array >= this->size() * handles.chunk_size
             \param  handles the collection of linear spaces
             */
            void save(void *dst, const linear_handles &handles ) const;
            
            
            //! load interleaved data from an array of bytes
            /**
             \param src the address of interleaved bytes
             \param num the number of interleaved bytes
             \param handles the collecton of linear spaces
             */
            void   load(linear_handles &handles, void *src) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
