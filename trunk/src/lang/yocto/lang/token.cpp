#include "yocto/lang/token.hpp"
namespace yocto
{
    namespace lang
    {
        
        t_char *t_char:: acquire(code_type value)
        {
            t_char *ans = object::acquire1<t_char>();
            ans->code   = value;
            return ans;
        }
        
        void t_char:: release(t_char *ch) throw()
        {
            assert(ch);
            assert(0==ch->next);
            assert(0==ch->prev);
            object::release1<t_char>(ch);
        }
    }
    
}

#include "yocto/exception.hpp"
#include <iostream>

namespace yocto
{
    namespace lang
    {

        token:: token() throw() : t_list()
        {
        }
        
        token:: ~token() throw()
        {
            clear();
        }
        
        void token:: clear() throw()
        {
            delete_with( t_char::release );
        }
        
        token:: token( const token &other ) : t_list()
        {
            try
            {
                for( const t_char *ch = other.head;ch;ch=ch->next)
                {
                    push_back( t_char::acquire(ch->code) );
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

        token::token( const string &s ) : t_list()
        {
            try
            {
                const size_t n = s.size();
                for(size_t i=0;i<n;++i)
                {
                    push_back( t_char::acquire(s[i]) );
                }
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        token::token( const char *s ) : t_list()
        {
            try
            {
                const size_t n = length_of(s);
                for(size_t i=0;i<n;++i)
                {
                    push_back( t_char::acquire(s[i]) );
                }
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        token:: token( const code_type code ) : t_list()
        {
            push_back( t_char::acquire(code) );
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

        token & token:: operator=( const code_type s )
        {
            token tmp(s);
            swap_with(tmp);
            return *this;
        }
        
        string token:: to_string(size_t nskip,size_t ntrim) const
        {
            const size_t ngone = nskip+ntrim;
            if(ngone>size)
                throw exception("token.to_string(not enough char)");
            string ans;
            const t_char *ch = head;
            for(size_t i=nskip;i>0;--i,ch=ch->next)
                ;
            for(size_t i=size-ngone;i>0;--i,ch=ch->next)
            {
                ans.append(char(ch->code));
            }
            return ans;
        }
        
        void token:: skip(size_t n) throw()
        {
            assert(n<=size);
            for(size_t i=n;i>0;--i) t_char::release( pop_front() );
        }
        
        void token:: trim(size_t n) throw()
        {
            assert(n<=size);
            for(size_t i=n;i>0;--i) t_char::release( pop_back() );
        }

        std::ostream & operator<<( std::ostream &os, const token &tk)
        {
            for(const t_char *ch = tk.head;ch;ch=ch->next)
            {
                os << char(ch->code);
            }
            return os;
        }

    }
}
