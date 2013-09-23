#ifndef YOCTO_MEMORY_PAGE_INCLUDED
#define YOCTO_MEMORY_PAGE_INCLUDED 1

#include "yocto/object.hpp"

namespace yocto
{
    
    namespace memory
    {
        
        class page
        {
        public:
            YOCTO_MAKE_OBJECT
            
            page(size_t nbytes);
            ~page() throw();
            
            struct block_t
            {
                block_t *prev;
                block_t *next;
                size_t   size;
                page    *from;
            };
            static const size_t block_size = sizeof(block_t);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(page);
            size_t         nblk;
            block_t       *head;
            block_t       *tail;
                        
        public:
            page *next;
            page *prev;
        };
        
    }
    
}

#endif
