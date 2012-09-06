#ifndef YOCTO_SPADE_DATA_BLOCK_INCLUDED
#define YOCTO_SPADE_DATA_BLOCK_INCLUDED 1

#include "yocto/spade/linear.hpp"

namespace yocto
{
    
    namespace spade
    {
        class data_block
        {
        public:
            explicit data_block( linear &l );
            virtual ~data_block() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(data_block);
            size_t size; //!< l.bytes
            void  *data; //!< >= size
        };
    }
    
}

#endif
