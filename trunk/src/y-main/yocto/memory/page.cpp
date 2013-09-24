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
        
        bool page:: is_empty(void) const throw()
        {
            return 0 == head->from && head->next == tail;
        }
        
        page:: page( size_t nbytes ) :
        safe(false),
        nblk( 2 + __rpbs(nbytes)/block_size ),
        head( locked::instance().try_acquire_as<block_t>(nblk,(bool&)safe) ),
        tail( &head[nblk]-1 ),
        next(0),
        prev(0)
        {
            assert(nblk>2);
            
            //------------------------------------------------------------------
            // format head
            //------------------------------------------------------------------
            head->prev = 0;
            head->next = tail;
            head->from = 0;
            head->size = (nblk-2) * block_size;
            
            assert(head->size == block_size * (static_cast<ptrdiff_t>(tail-head)-1));
            //------------------------------------------------------------------
            // format tail
            //------------------------------------------------------------------
            tail->prev = head;
            tail->next = 0;
            tail->from = this;
            tail->size = 0;
        }
        
        page:: ~page() throw()
        {
            assert(is_empty());
            locked::location()->release_as<block_t>(head, nblk);
        }
        
        void *page:: acquire(size_t &n) throw()
        {
            block_t *blk = head;
            assert(blk!=NULL);
            while(blk!=tail)
            {
                assert(blk->next!=0);
                if(0 == blk->from && blk->size>=n)
                {
                    //==========================================================
                    // found a block !
                    //==========================================================
                    const size_t usr_aligned = __rpbs(n);
                    const size_t usr_nblocks = usr_aligned/block_size;
                    const size_t blk_aligned = blk->size;
                    const size_t blk_nblocks = blk_aligned/block_size;
                    const size_t split_limit = usr_nblocks+1;
                    if( blk_nblocks>split_limit)
                    {
                        block_t *next = blk->next; assert(next!=0);
                        block_t *dest = blk+split_limit;
                        blk->next  = dest;
                        dest->prev = blk;
                        dest->next = next;
                        dest->from = 0;     // mark as free
                        next->prev = dest;
                        
                        blk->size  = usr_aligned;
                        dest->size = block_size*(blk_nblocks-split_limit);
                        assert( (static_cast<ptrdiff_t>(dest->next-dest)-1) * block_size == dest->size );
                    }
                    
                    blk->from = this;
                    n         = blk->size;
                    void   *p = &blk[1];
                    
                    memset(p,0,n);
                    return p;
                }
                
                blk = blk->next;
            }
            return 0;
        }
        
        page *page:: release( void *p ) throw()
        {
            assert(p);
            block_t *blk = static_cast<block_t *>(p) - 1;
            assert(blk->from); // must be allocated
            assert(blk->next); // block can't be a tail
            page    *P   = blk->from;
            
            blk->from = 0;
            if(blk->next->from == 0 )
            {
                __merge_next_of(blk);
            }
            
            if(blk->prev&&0==blk->prev->from)
            {
                __merge_next_of(blk->prev);
            }
            
            return P;
        }
        
        void page:: __merge_next_of( block_t *blk ) throw()
        {
            assert(blk);
            assert(0==blk->from);
            assert(blk->next);
            assert(0==blk->next->from);
            assert(blk->next->next!=0);
            blk->next = blk->next->next;
            blk->next->prev = blk;
            blk->size = (static_cast<ptrdiff_t>(blk->next-blk)-1) * block_size;
        }
        
    }
}
