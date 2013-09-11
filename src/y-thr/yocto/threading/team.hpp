#ifndef YOCTO_THREADING_TEAM_INCLUDED
#define YOCTO_THREADING_TEAM_INCLUDED 1

#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/threading/context.hpp"

namespace yocto
{
    namespace threading
    {
        
        class team : public layout
        {
        public:
            explicit team();
            explicit team(size_t num_threads, size_t off_threads = 0);
            virtual ~team() throw();
            
            context       & operator[](size_t rank) throw();
            const context & operator[](size_t rank) const throw();

        private:
            mutex     access;
            condition cycle;
            bool      stop;
            size_t wlen;
            void  *wksp;
            size_t built;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(team);
            
            void startup();
            void cleanup() throw();
            static void launch(void *args) throw();
            void engine( context & ) throw();
        };
        
    }
}

#endif
