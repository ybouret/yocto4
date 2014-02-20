#ifndef YOCTO_THREADING_CONTEXT_INCLUDED
#define YOCTO_THREADING_CONTEXT_INCLUDED 1

#include "yocto/lockable.hpp"
#include "yocto/container/vslot.hpp"

namespace yocto
{
    namespace threading
    {
        
        //! context of current thread
        class context : public vslot
        {
        public:
            const size_t rank;   //!< 0..size-1
            const size_t indx;   //!< rank+1, for information
            const size_t size;   //!< size of the crew
            lockable    &access; //!< common mutex for synchronization
            
            explicit context( size_t r, size_t s, lockable &lock_ref) throw();
            virtual ~context() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(context);
        };
        
    }
}

#endif

