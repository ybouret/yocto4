#ifndef YOCTO_SWAMP_DATA_BLOCK_INCLUDED
#define YOCTO_SWAMP_DATA_BLOCK_INCLUDED 1

#include "yocto/swamp/linear.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto 
{
    namespace swamp 
    {
        class data_block : public object
        {
        public:
            explicit data_block( linear_base &linear_object );
            virtual ~data_block() throw();
            
            typedef shared_ptr<data_block> ptr;
            
        private:
            size_t size;
            void  *base;
            YOCTO_DISABLE_COPY_AND_ASSIGN(data_block);
        };
    }
    
}

#endif
