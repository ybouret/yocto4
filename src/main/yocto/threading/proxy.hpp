#ifndef YOCTO_THREADING_PROXY_INCLUDED
#define YOCTO_THREADING_PROXY_INCLUDED 1

#include "yocto/threading/thread.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace threading
    {
#if USE_OLD_THREAD
        //! for cheap threading
        class proxy
        {
        public:
            explicit proxy() throw();
            virtual ~proxy() throw();
            
            void launch( threading::thread::proc_t proc, void *data );
            void finish() throw();
            void on_cpu(size_t id);
            
            template <typename FUNC>
            void trigger( FUNC &fn )
            {
                xdata[0] = (void *)&fn;
                launch( proxy::execute<FUNC>, this );
            }
            
            template <typename FUNC, typename T>
            void trigger( FUNC &fn, T &x)
            {
                xdata[0] = (void *)&fn;
                xdata[1] = (void *)&x;
                launch( proxy::execute2<FUNC,T>, this );
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(proxy);
            void    *xdata[4]; //!< for external argument
            uint64_t block[ YOCTO_U64_FOR_ITEM(threading::thread) ];
            void clear() throw();
            template <typename FUNC> static inline
            void   execute( void *args ) throw()
            {
                assert(args!=0);
                proxy &self = *(proxy *)args;
                assert( self.xdata[0] );
                union
                {
                    void *item;
                    FUNC *func;
                } alias = { self.xdata[0] };
                assert(alias.func);
                FUNC &fn = * alias.func;
                fn();
            }
            
            template <typename FUNC,typename T> static inline
            void   execute2( void *args ) throw()
            {
                assert(args);
                proxy &self = *(proxy *)args;
                assert( self.xdata[0] );
                assert( self.xdata[1] );
                union
                {
                    void *item;
                    FUNC *func;
                } alias = { self.xdata[0] };
                assert(alias.func);
                FUNC &fn = * alias.func;
                T    &x  = *(T *) self.xdata[1];
                fn(x);
            }
        };
#endif
    }
    
}

#endif
