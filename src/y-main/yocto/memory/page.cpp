#include "yocto/memory/page.hpp"
#include "yocto/object.hpp"
#include "yocto/memory/locked.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>

namespace yocto
{
    namespace memory
    {
        
        static inline
        size_t __rpbs( size_t n )
        {
            if(n<=0)
                n = page::block_size;
            else
                while( 0 != (n%page::block_size) ) ++n;
            return n;
        }
        
        page:: page( size_t nbytes ) :
        nblk( 2 + __rpbs(nbytes)/block_size ),
        head( locked::instance().acquire_as<block_t>(nblk) ),
        tail( &head[nblk]-1 ),
        next(0),
        prev(0)
        {
            assert(nblk>2);
            
            //------------------------------------------------------------------
            // format
            //------------------------------------------------------------------
            head->prev = 0;
            head->next = tail;
            head->from = 0;
            head->size = (nblk-2) * block_size;
            
            tail->prev = head;
            tail->next = 0;
            tail->from = this;
            tail->size = 0;
        }
        
        page:: ~page() throw()
        {
            locked::location()->release_as<block_t>(head, nblk);
        }
        
    }
}
