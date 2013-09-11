#include "yocto/threading/team.hpp"
#include "yocto/threading/proxy.hpp"
#include "yocto/memory/global.hpp"

#include <iostream>

namespace yocto
{
    namespace threading
    {
        
        namespace
        {
            class member : public proxy, public context
            {
            public:
                team   &parent;
                
                
                inline member(size_t rank,
                              size_t size,
                              mutex &access,
                              team  &owner) throw() :
                proxy(),
                context(rank,size,access),
                parent(owner)
                {
                }
                
                inline ~member() throw()
                {
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(member);
            };
        }
        
        
        void team:: startup()
        {
            
            //------------------------------------------------------------------
            // initialize this thread
            //------------------------------------------------------------------
            thread::assign_cpu( thread::get_current_handle(), cpu_index_of(0) );
            
            //------------------------------------------------------------------
            // initialize members
            //------------------------------------------------------------------
            member *m = static_cast<member *>(wksp);
            for(size_t rank=0;rank<size;++rank)
                new ( &m[rank] ) member(rank,size,access,*this);
            
            //------------------------------------------------------------------
            // launch engines
            //------------------------------------------------------------------
            std::cerr << "Launching Threads" << std::endl;
            try
            {
                while(built<size)
                {
                    member *M = &m[built];
                    M->launch( team::launch, M );
                    ++built;
                    {
                        scoped_lock guard(access);
                        M->on_cpu( cpu_index_of(built) );
                    }
                }
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }
        
        
        void team:: cleanup() throw()
        {
            
            stop = true;
            
            member *m = static_cast<member *>(wksp)+built;
            while(--m,built-->0)
            {
                m->finish();
            }
            
            memory::kind<memory::global>::release(wksp, wlen);
            
        }
        
        
        team:: ~team() throw()
        {
            cleanup();
        }
        
#define Y_TEAM_CTOR()     \
access( "team::access" ), \
cycle(), \
stop(false), \
wlen( size * sizeof(member) ),                  \
wksp( memory::kind<memory::global>::acquire(wlen) ), \
built(0)
        
        
        
        team:: team() :
        layout(),
        Y_TEAM_CTOR()
        {
            startup();
        }
        
        
        team:: team( size_t num_threads, size_t off_threads) :
        layout(num_threads,off_threads),
        Y_TEAM_CTOR()
        {
            startup();
        }
        
        void team:: launch( void *args ) throw()
        {
            assert(args);
            member &m = *(member *)args;
            m.parent.engine( m );
        }
        
               
        context & team:: operator[](size_t rank) throw()
        {
            assert(rank<size);
            return *(static_cast<member *>(wksp)+rank);
        }
        
        const context & team:: operator[](size_t rank) const throw()
        {
            assert(rank<size);
            return *(static_cast<member *>(wksp)+rank);
        }
        
        
        void team:: engine( context &ctx ) throw()
        {
            {
                scoped_lock guard(ctx.access);
                std::cerr << "running rank " << ctx.rank << std::endl;
            }
            
            
        }

        
    }
}
