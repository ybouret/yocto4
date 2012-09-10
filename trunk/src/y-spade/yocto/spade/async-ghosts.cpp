#include "yocto/spade/async-ghosts.hpp"
#include "yocto/spade/memory.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        async_ghosts:: ~async_ghosts() throw()
        {
            if( ibuffer )
            {
                memory_release(ibuffer,iolen);
                obuffer = 0;
                iobytes = 0;
            }
        }
        
        async_ghosts:: async_ghosts( ghost::position pos ) throw() :
        inner( pos ),
        outer( pos ),
        peer(-1),
        ibuffer(0),
        obuffer(0),
        iobytes(0),
        iolen(0)
        {
        }
        
        void async_ghosts:: allocate_for( const linear_handles &handles )
        {
            assert( 0 == ibuffer );
            assert( 0 == obuffer );
            assert( 0 == iobytes );
            assert( 0 == iolen   );
            const size_t n = num_offsets * handles.interleaved();
            if( n > 0 )
            {
                const size_t buff_size = n << 1;
                iolen   = buff_size;
                ibuffer = memory_acquire<uint8_t>(iolen);
                iobytes = buff_size;
                obuffer = ibuffer + iobytes;
            }
        }
        
        
        size_t async_ghosts:: inner_store( const linear &handle ) throw()
        {
            assert(num_offsets>0);
            assert(handle.item_size() * num_offsets<=iobytes);
            uint8_t    *p=ibuffer;
            for( size_t i=num_offsets;i>0;--i)
            {
                handle.async_store(p,inner[i]);
                assert(p<=ibuffer+iobytes);
            }
            assert( static_cast<size_t>(p-ibuffer) == handle.item_size() * num_offsets );
            return static_cast<size_t>(p-ibuffer);
        }
        
        void async_ghosts:: outer_query( linear &handle ) throw()
        {
            assert(num_offsets>0);
            assert(handle.item_size() * num_offsets<=iobytes);
            const uint8_t *p = obuffer;
            for( size_t i=num_offsets;i>0;--i)
            {
                handle.async_query(p,outer[i]);
                assert(p<=obuffer+iobytes);
            }
            
        }
        
        size_t async_ghosts:: inner_store( const linear_handles &handles ) throw()
        {
            assert(num_offsets>0);
            assert(handles.interleaved() * num_offsets<=iobytes);
            
            const size_t num_handles = handles.size();
            uint8_t     *p           = ibuffer;
            
            for( size_t i=num_offsets;i>0;--i)
            {
                const size_t j = inner[i];
                for( size_t k=num_handles;k>0;--k)
                {
                    assert(handles[k]);
                    handles[k]->async_store(p,j);
                    assert(handles[k]);
                }
            }
            assert( static_cast<size_t>(p-ibuffer)== handles.interleaved() * num_offsets);
            return static_cast<size_t>(p-ibuffer);
        }
        
        
        void async_ghosts::  outer_query( linear_handles &handles ) throw()
        {
            assert(num_offsets>0);
            assert(handles.interleaved() * num_offsets<=iobytes);
            const size_t   num_handles = handles.size();
            const uint8_t *p           = obuffer;
            for( size_t i=num_offsets;i>0;--i)
            {
                const size_t j = outer[i];
                for( size_t k=num_handles;k>0;--k)
                {
                    assert(handles[k]);
                    handles[k]->async_query(p,j);
                    assert(handles[k]);
                }
            }
        }
    }
    
}
