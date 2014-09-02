#include "yocto/lang/source.hpp"

namespace yocto
{
    namespace lang
    {
        source:: ~source() throw()
        {
        }
        
        source:: source() throw() : cache()
        {
        }
        
        void source:: unget(t_char *ch) throw()
        {
            assert(ch);
            cache.push_front(ch);
        }
        
        t_char * source:: get( ios::istream &fp )
        {
            if(cache.size>0)
            {
                return cache.pop_front();
            }
            else
            {
                t_char *ch = t_char::acquire();
                char C;
                try
                {
                    if( !fp.query(C) )
                    {
                        t_char::release(ch);
                        return NULL;
                    }
                    ch->code = C;
                    return ch;
                }
                catch(...)
                {
                    t_char::release(ch);
                    throw;
                }
            }
        }
        
        
        void source:: unget(token &tk) throw()
        {
            cache.merge_front(tk);
        }
        
        void source:: uncpy(const token &tk)
        {
            token tmp(tk);
            cache.merge_front(tmp);
        }

        const t_char *source:: peek( ios::istream &fp )
        {
            if(cache.size>0)
            {
                return cache.head;
            }
            else
            {
                t_char *ch = t_char::acquire();
                char C;
                try
                {
                    if( !fp.query(C) )
                    {
                        t_char::release(ch);
                        return false;
                    }
                    ch->code = C;
                    cache.push_back(ch);
                    return ch;
                }
                catch(...)
                {
                    t_char::release(ch);
                    throw;
                }
            }
        }
        
        void source:: skip(size_t n) throw()
        {
            assert(n<=cache.size);
            while(n-->0) delete cache.pop_front();
        }
        
    }
    
}