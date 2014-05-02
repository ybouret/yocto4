#include "yocto/threading/window.hpp"

namespace yocto
{
    
    namespace threading
    {
        
        window:: ~window() throw() {}
        
        window:: window( const context &ctx, size_t length, size_t offset ) throw() :
        start(0),
        count(0),
        final(0)
        {
            // initialize
            const size_t size = ctx.size;
            size_t       todo = length/size;
            
            //forward
            for(size_t r=1;r<=ctx.rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
            }
            
            // done
            (size_t &)start = offset;
            (size_t &)count = todo;
            (size_t &)final = (start+count)-1;
            
        }
        
        window:: window( const window &w ) throw() :
        start(w.start),
        count(w.count),
        final(w.final)
        {
        }
        
    }
    
}
