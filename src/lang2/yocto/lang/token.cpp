#include "yocto/lang/token.hpp"

namespace yocto
{
    namespace lang
    {
        t_char::  t_char() throw() : next(0), prev(0), code(0) {}
        
        t_char:: ~t_char() throw() {}
        
        t_char::  t_char(code_t C) throw() :  next(0), prev(0), code(C) {}

    }
}

#include <iostream>

namespace yocto
{
    
    namespace lang
    {
        
      
        
        token:: ~token() throw() {}
        
        token:: token() throw() : t_list() {}
        
        token:: token( const token &other ) : t_list()
        {
            for(const t_char *ch = other.head; ch; ch=ch->next)
            {
                push_back(new t_char(ch->code) );
            }
        }
        
        token & token:: operator=( const token &other )
        {
            if( this != &other )
            {
                token tmp(other);
                swap_with(tmp);
            }
            return *this;
        }
        
        std:: ostream & operator<<( std::ostream &os, const token &tkn )
        {
            for(const t_char *ch = tkn.head; ch; ch=ch->next)
            {
                os << char(ch->code);
            }
            return os;
        }

        token:: token(const char C) : t_list()
        {
            push_back( new t_char(C) );
        }

        token:: token( const string &s ) : t_list()
        {
            for(size_t i=0;i<s.size();++i)
            {
                push_back( new t_char(s[i]) );
            }
        }

        token:: token( const string &s ) : t_list()
        {
            for(size_t i=0;i<s.size();++i)
            {
                push_back( new t_char(s[i]) );
            }
        }

        string token:: to_string() const
        {
            string ans(size,as_capacity);
            for(const t_char *ch = head;ch; ch=ch->next)
            {
                ans.append(char(ch->code));
            }
            return ans;
        }

        token:: token( const char *s ) : t_list()
        {
            const size_t n = length_of(s);
            for(size_t i=0;i<n;++i)
            {
                push_back( new t_char(s[i]) );
            }

        }
    }

}
