#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

#include "yocto/threading/proxy.hpp"
#include "yocto/code/cast.hpp"

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
            _cast::from<threading::thread>( &block[0] )->join();
            clear();
        }

    }
}
