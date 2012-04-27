#include "yocto/swamp/data-block.hpp"

namespace yocto
{
    namespace swamp 
    {
        data_block:: ~data_block() throw()
        {
            memory::kind<memory::global>::release( base, size );
        }
        
        data_block:: data_block( linear_base &linear_object ) :
        size( linear_object.bytes ),
        base( memory::kind<memory::global>::acquire( size ) )
        {
            linear_object.link( base );
        }
        
        
    }
}
