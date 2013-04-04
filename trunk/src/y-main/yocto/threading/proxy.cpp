#include "yocto/threading/proxy.hpp"

#include <cstring>
#include <new>

namespace yocto
{
    namespace threading
    {
        void proxy:: clear() throw()
        {
            memset( xdata, 0, sizeof(xdata) );
            memset( block, 0, sizeof(block) );
        }
        
        proxy:: proxy() throw() :
        xdata(),
        block()
        {
            clear();
        }
        
        proxy:: ~proxy() throw()
        {
            clear();
        }
        
        void proxy:: launch( threading::thread::proc_t proc, void *data )
        {
            try
            {
                new ( &block[0] ) threading::thread(proc,data);
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        void proxy:: finish() throw()
        {
            static_cast<threading::thread *>( (void*)&block[0] )->join();
            clear();
        }

    }
}
