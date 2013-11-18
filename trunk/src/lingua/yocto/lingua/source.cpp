#include "yocto/lingua/source.hpp"
#include <cstring>

namespace yocto
{
    
    namespace lingua
    {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // source
        //
        ////////////////////////////////////////////////////////////////////////
        source:: ~source() throw()
        {
            detach();
        }
        
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
        source:: source() throw() :
        cache(),
        ppInp(0),
        block()
        {
            init();
        }
        
        void source:: init() throw()
        {
            memset(block,0,sizeof(block));
            ppInp = 0;
            cache.clear();
        }

        void source:: detach() throw()
        {
            if(ppInp)
            {
                ppInp->~input();
                init();
            }
        }

        void source:: attach(const input &fp) throw()
        {
            detach();
            ppInp = new (block) input(fp);
        }

        
        void source:: unget(t_char *ch) throw()
        {
            assert(ch!=NULL);
            cache.push_front(ch);
        }
        
        size_t  source:: cache_size() const throw()
        {
            return cache.size;
        }

        
        void source:: unget(token &token) throw()
        {
            cache.merge_front(token);
            assert(0==token.size);
        }
        
        void source:: uncpy(const token &t)
        {
            token tmp(t);
            cache.merge_front(tmp);
        }
        
        t_char *source:: get()
        {
            if( cache.size )
            {
                return cache.pop_front();
            }
            else
            {
                //--------------------------------------------------------------
                // no cache: try input
                //--------------------------------------------------------------
                t_char *ch = t_char::acquire();
                try
                {
                    if(ppInp && (**ppInp).query(ch->data) )
                        return ch;
                }
                catch(...)
                {
                    t_char::release(ch);
                    throw;
                }
                
                //--------------------------------------------------------------
                // no cache, no input
                //--------------------------------------------------------------
                t_char::release(ch);
                return 0;
                
            }
        }
        
        
        void source:: skip(size_t n) throw()
        {
            assert(n<=cache.size);
            while(n-->0) t_char::release(cache.pop_front());
        }
        
        bool source:: is_active()
        {
            t_char *ch = get();
            if( ch )
            {
                cache.push_front(ch);
                return true;
            }
            else
                return false;
        }

        char source:: peek() const throw()
        {
            assert( cache_size() > 0 );
            return cache.head->data;
        }
     
        bool    source:: is_attached() const throw()
        {
            return ppInp != 0;
        }

        
    }
    
}


#include "yocto/ios/icstream.hpp"

namespace yocto
{
    namespace lingua
    {
        void source:: attach_stdin()
        {
            const input fp( new ios::icstream(ios::cstdin) );
            attach(fp);
        }
    }
}
