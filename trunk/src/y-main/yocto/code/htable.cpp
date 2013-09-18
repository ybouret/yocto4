#include "yocto/code/htable.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/code/utils.hpp"
#include <cerrno>

namespace yocto
{
    size_t htable:: compute_slots_for(size_t &items)
    {
        //----------------------------------------------------------------------
        // compute the minimum #items according to specs
        //----------------------------------------------------------------------
        static const size_t min_items = min_slots * load_factor;
        if(items<=0) return 0;
        
        //----------------------------------------------------------------------
        // initialize #nodes
        //----------------------------------------------------------------------
        size_t nodes = max_of(min_items,items);
        
        //----------------------------------------------------------------------
        // adjust to load_factor
        //----------------------------------------------------------------------
        while( 0 != ( nodes % load_factor) )
            if( ++nodes < items ) throw libc::exception( ERANGE, "htable: #nodes overflow, level-1" );
        
        //----------------------------------------------------------------------
        // get the corresponding prime size
        //----------------------------------------------------------------------
        size_t slots = next_prime( nodes/load_factor );
        if(slots<min_slots) throw libc::exception( ERANGE, "htable: #slots overflow" );
        
        //----------------------------------------------------------------------
        // recompute the effective #nodes
        //----------------------------------------------------------------------
        nodes = slots * load_factor;
        if( nodes < items )
            throw libc::exception( ERANGE, "htable: #nodes overflow, level-2");
        
        items = nodes;
        return slots;
    }
    
    
    size_t htable:: compute_slots_for(size_t &items, size_t &sub_prime)
    {
        sub_prime = 0;
        if(items<=0) return 0;
        
        //----------------------------------------------------------------------
        // compute the slots
        //----------------------------------------------------------------------
        const size_t slots = max_of<size_t>(items,3) * load_factor;
        const size_t count = slots/load_factor;
        if(count<items) throw libc::exception( ERANGE, "htable: #count overflow");
        
        sub_prime = prev_prime(slots-1);
        items     = count;
        return slots;
    }

    
}

