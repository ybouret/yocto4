#include "yocto/lingua/token.hpp"
#include <iostream>

namespace yocto
{

    namespace lingua
    {
        t_char:: ~t_char() throw()
        {
        }

        t_char:: t_char(const uint8_t C) throw() : next(0), prev(0), code(C) {}
        t_char:: t_char(const t_char &other) throw() : next(0), prev(0), code(other.code) {}

        std::ostream & operator<<( std::ostream &os, const t_char &ch )
        {
            os << char(ch.code);
            return os;
        }
    }
}

#include "yocto/string/conv.hpp"

namespace yocto
{

    namespace lingua
    {
        token::  token() throw() : t_char::list_type() {}
        token:: ~token() throw() {}

        token:: token(const token &other) : t_char::list_type(other) {}
        token & token:: operator=(const token &other )
        {
            token tmp(other);
            swap_with(tmp);
            return *this;
        }

        token:: token(const char C) : t_char::list_type()
        {
            push_back( new t_char(C) );
        }

        token:: token(const char *s) : t_char::list_type()
        {
            const size_t n = length_of(s);
#define YTKN_ADD(I) push_back( new t_char(s[I]) )
            YOCTO_LOOP_FUNC(n,YTKN_ADD,0);
        }

        token:: token(const string &s) : t_char::list_type()
        {
            YOCTO_LOOP_FUNC(s.size(),YTKN_ADD,0);
#undef YTKN_ADD
        }

        std::ostream & operator<<( std::ostream &os, const token &tkn )
        {
            for(const t_char *ch=tkn.head;ch;ch=ch->next)
            {
                os << (*ch);
            }
            return os;
        }

        string token:: to_string(const size_t nskip,const size_t ntrim) const
        {

            string ans;
            const size_t ndrop = nskip+ntrim;
            if(ndrop<size)
            {
                const t_char *ch = head;
                for(register size_t i=nskip;i>0;--i)
                {
                    ch=ch->next;
                }
                for(register size_t i=size-ndrop;i>0;--i,ch=ch->next)
                {
                    ans.append(ch->code);
                }
            }
            return ans;
        }

        string token:: to_cstring(const size_t nskip,const size_t ntrim) const
        {

            string ans;
            const size_t ndrop = nskip+ntrim;
            if(ndrop<size)
            {
                const t_char *ch = head;
                for(register size_t i=nskip;i>0;--i)
                {
                    ch=ch->next;
                }
                for(register size_t i=size-ndrop;i>0;--i,ch=ch->next)
                {
                    strconv::append_cchar(char(ch->code), ans);
                }
            }
            return ans;
        }


    }

}
