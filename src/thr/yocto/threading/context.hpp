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
            
            template <typename WINDOW>
            inline void create( size_t length, size_t offset )
            {
                const WINDOW win(*this,length,offset);
                this->make<WINDOW>(win);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(context);
        };
        
        
        //! for use with no threads
        class single_context : public faked_lock, public context
        {
        public:
            explicit single_context() throw();
            virtual ~single_context() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(single_context);
        };
        
    }
}

#endif

