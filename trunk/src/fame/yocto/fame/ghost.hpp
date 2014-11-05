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
            
            //! copy local data
            static void copy(linear_space       &dst,
                             const ghost        &gdst,
                             const linear_space &src,
                             const ghost        &gsrc) throw();
            
            //! save interleaved data into an array of bytes
            /**
             \param  dst the adress of the array
             \param  num number of bytes
             \param  handles the collection of linear spaces
             \return the number of bytes, for MPI
             */
            size_t save(void *dst, const size_t num, const array<linear_handle> &handles ) const;
            
            
            //! load interleaved data from an array of bytes
            /**
             \param src the address of interleaved bytes
             \param num the number of interleaved bytes
             \param handles the collecton of linear spaces
             */
            void   load(void *src, const size_t num, array<linear_handle> &handles ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
