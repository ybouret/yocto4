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
        // token
        //
        ////////////////////////////////////////////////////////////////////////
        token:: token() throw()
        {
        }
        
        void token:: clear() throw()
        {
            while(size) t_char::release( pop_back() );
        }
        
        
        token:: ~token() throw()
        {
            clear();
        }
        
        token:: token( const token &other ) 
        {
            try
            {
                for( const t_char *ch = other.head;ch;ch=ch->next)
                {
                    push_back( t_char::acquire(ch->data) );
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
            token tmp(other);
            swap_with(tmp);
            return *this;
        }
        
        string token:: to_string( size_t nskip,size_t ntrim) const
        {
            const size_t ndel = nskip+ntrim;
            if(ndel>=size)
                return string();
            else
            {
                size_t n = size - ndel; assert(n>0);
                string ans(n,as_capacity);
                const t_char *ch = head;
                while(nskip-->0)
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
        
        token:: token(const string &s)
        {
            try
            {
                const size_t n = s.size();
                for(size_t i=0;i<n;++i)
                    push_back( t_char::acquire(s[i]) );
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        token:: token(const char *s)
        {
            try
            {
                const size_t n = length_of(s);
                for(size_t i=0;i<n;++i)
                    push_back( t_char::acquire(s[i]) );
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

        
        token & token:: operator=( const string &s )
        {
            token tmp(s);
            swap_with(tmp);
            return *this;
        }

        token & token:: operator=( const char *s )
        {
            token tmp(s);
            swap_with(tmp);
            return *this;
        }
        
        void token:: skip() throw()
        {
            assert(size>0);
            t_char::release( pop_front() );
        }

        void token:: trim() throw()
        {
            assert(size>0);
            t_char::release( pop_back() );
        }
        
    }
    
}
