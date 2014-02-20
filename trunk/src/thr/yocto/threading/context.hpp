#ifndef YOCTO_THREADING_CONTEXT_INCLUDED
#define YOCTO_THREADING_CONTEXT_INCLUDED 1

#include "yocto/lockable.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/functor.hpp"

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
            const size_t size;   //!< the size=#threads
            lockable    &access; //!< common lock for synchronization
            
            typedef functor<void,TL1(context&)> kernel;
            
            explicit context( size_t r, size_t s, lockable &lock_ref) throw();
            virtual ~context() throw();
            
            // build a window
            template <typename WINDOW>
            inline void create( size_t length, size_t offset )
            {
                this->build<WINDOW,context&,size_t,size_t>(*this,length,offset);
            }
            
            //! create WINDOWs in self
            /** 
             which must have a 'size' field and a
             context & operator[](rank) function
             */
            template <typename CONTAINER,typename WINDOW>
            static inline void dispatch(CONTAINER &self, size_t length, size_t offset)
            {
                for(size_t rank=0;rank<self.size;++rank)
                {
                    self[rank].template create<WINDOW>(length,offset);
                }
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
        
        //! for use with server
        class context_batch
        {
        public:
            explicit context_batch(size_t n, lockable &lock_ref);
            virtual ~context_batch() throw();
            
            const size_t size;
            
            context       & operator[](size_t rank) throw();
            const context & operator[](size_t rank) const throw();
            
            //! make windows in contexts data
            template <typename WINDOW>
            inline void dispatch( size_t length, size_t offset )
            {
                context::dispatch<context_batch,WINDOW>(*this,length,offset);
            }

            
        private:
            size_t   cnt; //!< for memory
            context *ctx; //!< localization
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(context_batch);
        };
        
        
    }
}

#endif

