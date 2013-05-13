#include "yocto/lingua/source.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // input
        //
        ////////////////////////////////////////////////////////////////////////
        input:: ~input() throw()
        {
        }
        
        input:: input( const string &id, const istream_ptr &fp ) :
        name(id),
        iptr(fp)
        {
        }
        
        input:: input( const input &other ) :
        name( other.name ),
        iptr( other.iptr )
        {
        }
        
        bool input:: query( char &C )
        {
            return iptr->query(C);
        }
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // source
        //
        ////////////////////////////////////////////////////////////////////////
        source:: ~source() throw()
        {
        }
        
        source:: source( t_cache &p ) :
        tpool(p),
        cache(tpool),
        instk(2,as_capacity)
        {
        }
        
        
        void source:: unget(t_char *ch) throw()
        {
            assert(ch!=NULL);
            cache.push_front(ch);
        }
        
        void source:: unget(token &token) throw()
        {
            cache.merge_front(token);
            assert(0==token.size);
        }
        
        void source:: uncpy(const token &t)
        {
            token tmp(t,tpool);
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
                // no cache: try inputs
                //--------------------------------------------------------------
                t_char *ch = tpool.create(0);
                try
                {
                    while(instk.size()>0)
                    {
                        input  &curr = instk.peek();
                        if(curr.query(ch->data))
                            return ch;
                        //-- done with this input
                        std::cerr << "-<" << curr.name << ">" << std::endl;
                        instk.pop();
                    }
                }
                catch(...)
                {
                    tpool.store(ch);
                    throw;
                }
                
                //--------------------------------------------------------------
                // no cache, no input
                //--------------------------------------------------------------
                tpool.store(ch);
                return 0;
                
            }
        }
        
        void  source:: push(const string &id, const istream_ptr &fp)
        {
            const input inp(id,fp);
            instk.push(inp);
        }
        
        
        
    }
    
}
