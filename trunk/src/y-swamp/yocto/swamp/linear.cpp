#include "yocto/swamp/linear.hpp"

namespace yocto 
{
    namespace swamp
    {
        linear_base::  linear_base( size_t num_bytes ) throw() : 
        bytes(num_bytes )
        {}
        
        linear_base:: ~linear_base() throw() {}
        
        size_t linear_base:: compute_bytes( size_t items, size_t the_item_size ) throw()
        {
            assert(items>0);
            assert(the_item_size>0);
            return items  * the_item_size;
        }
        
    }
    
}
