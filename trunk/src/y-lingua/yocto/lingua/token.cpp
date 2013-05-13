#include "yocto/lingua/token.hpp"
#include "yocto/object.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // t_char
        //
        ////////////////////////////////////////////////////////////////////////
        t_char * t_char:: acquire( char C )
        {
            t_char *ch = object::acquire1<t_char>();
            ch->data =C;
            return ch;
        }
        
        void t_char:: release(t_char *ch) throw()
        {
            assert(ch);
            object::release1<t_char>(ch);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // t_cache
        //
        ////////////////////////////////////////////////////////////////////////
        t_cache:: t_cache() throw() {}
        
        t_cache:: ~t_cache() throw()
        {
            kill();
        }
        
        void t_cache:: reserve( size_t n )
        {
            while(n-- > 0)
            {
                store( t_char::acquire() );
            }
        }
        
        t_char  * t_cache:: create(char C)
        {
            if(size>0)
            {
                t_char *ch  = query();
                ch->data = C;
                return ch;
            }
            else
                return t_char::acquire(C);
        }
        
        void    t_cache:: kill() throw()
        {
             delete_with( t_char::release );
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // token
        //
        ////////////////////////////////////////////////////////////////////////
        token:: token( t_cache &p ) throw() :
        cache(p)
        {
        }
        
        void token:: clear() throw()
        {
            while(size) cache.store( pop_back() );
        }
        
        
        token:: ~token() throw()
        {
            clear();
        }
        
        token:: token( const token &other ) :
        cache( other.cache )
        {
            try
            {
                for( const t_char *ch = other.head;ch;ch=ch->next)
                {
                    push_back( cache.create(ch->data) );
                }
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        token:: token( const token &other, t_cache &p ) :
        cache( p )
        {
            try
            {
                for( const t_char *ch = other.head;ch;ch=ch->next)
                {
                    push_back( cache.create(ch->data) );
                }
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

        
        token & token:: operator=( const token &other )
        {
            token tmp(cache);
            for(const t_char *ch = other.head; ch; ch=ch->next)
            {
                tmp.push_back( cache.create(ch->data) );
            }
            swap_with(tmp);
            return *this;
        }

        
        
        string token:: to_string( size_t skip,size_t trim) const
        {
            const size_t ndel = skip+trim;
            if(ndel>=size)
                return string();
            else
            {
                size_t n = size - ndel; assert(n>0);
                string ans(n,as_capacity);
                const t_char *ch = head;
                while(skip-->0)
                {
                    assert(ch!=NULL);
                    ch = ch->next;
                }
                while(n-->0)
                {
                    assert(ch!=NULL);
                    ans.append(ch->data);
                    ch = ch->next;
                }
                return ans;
            }
        }
        
        std::ostream & operator<<( std::ostream &os, const token &t)
        {
            for( const t_char *ch = t.head;ch;ch=ch->next)
            {
                os << (ch->data);
            }
            return os;
        }
        
        token:: token(t_cache &p, const string &s ):
        cache(p)
        {
            try
            {
                const size_t n = s.size();
                for(size_t i=0;i<n;++i)
                    push_back( cache.create(s[i]) );
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        token:: token(t_cache &p, const char *s ):
        cache(p)
        {
            try
            {
                const size_t n = length_of(s);
                for(size_t i=0;i<n;++i)
                    push_back( cache.create(s[i]) );
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

        
        token & token:: operator=( const string &s )
        {
            token tmp(cache,s);
            swap_with(tmp);
            return *this;
        }

        token & token:: operator=( const char *s )
        {
            token tmp(cache,s);
            swap_with(tmp);
            return *this;
        }

        
    }
    
}
