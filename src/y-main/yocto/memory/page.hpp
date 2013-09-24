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
            const bool safe;

            YOCTO_MAKE_OBJECT
            
            page(size_t nbytes);
            ~page() throw();
            
            struct block_t
            {
                block_t *prev;
                block_t *next;
                page    *from;
                size_t   size;
            };
            static const size_t block_size = sizeof(block_t);
            
            
            void        *acquire( size_t &n ) throw();
            static page *release( void *p ) throw();
            bool         is_empty(void) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(page);
            size_t         nblk;
            block_t       *head;
            block_t       *tail;
            
            static void __merge_next_of( block_t *blk ) throw();
            
        public:
            page *next;
            page *prev;
        };
        
    }
    
}

#endif
