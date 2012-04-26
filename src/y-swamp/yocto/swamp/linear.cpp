#include "yocto/swamp/linear.hpp"

namespace yocto 
{
    namespace swamp
    {
        linear_base::  linear_base() throw() {}
        linear_base:: ~linear_base() throw() {}

        size_t linear_base:: compute_bytes( size_t items, size_t item_size ) throw()
        {
            assert(items>0);
            assert(item_size>0);
            return items  * item_size;
        }
        
    }
    
}
