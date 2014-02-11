#ifndef YOCTO_THREADING_SERVER_INCLUDED
#define YOCTO_THREADING_SERVER_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace threading
    {
        
        
        
        class server
        {
        public:
            explicit server();
            virtual ~server() throw();
            
            typedef functor<void,null_type> job;
        
            class task
            {
            public:
                task *next;
                task *prev;
            private:
                job   work;
                task( const job & );
                ~task() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(task);
                friend class threading::server;
            };
            
            void enqueue( const job &J );
            
            
        private:
            threads workers;
            
        public:
            mutex  &access;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(server);
            core::list_of<task> tasks;
            core::pool_of<task> tpool;
        };
    }
}

#endif

