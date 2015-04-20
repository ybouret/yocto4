#include "yocto/lang/tchar-source.hpp"

namespace yocto
{
    namespace lang
    {

        t_char_source::  t_char_source() throw() : token() {}
        t_char_source:: ~t_char_source() throw() {}

        t_char *t_char_source:: get( ios::istream &fp )
        {

            if(size>0)
            {
                return pop_front();
            }
            else
            {
                t_char *ch = new t_char();
                try
                {
                    char C=0;
                    if(fp.query(C))
                    {
                        ch->code = C;
                        return ch;
                    }
                    else
                    {
                        delete ch;
                        return 0;
                    }
                }
                catch(...)
                {
                    delete ch;
                    throw;
                }
            }
        }

        const t_char * t_char_source:: peek( ios::istream &fp )
        {
            t_char *ch = get(fp);
            if(ch)
            {
                push_front(ch);
                return head;
            }
            else
                return 0;
        }


        void t_char_source:: skip(size_t n) throw()
        {
            assert(n<=size);
            while(n-->0)
            {
                delete pop_front();
            }
        }

        void t_char_source:: unget( t_char *ch ) throw()
        {
            assert(ch);
            push_front(ch);
        }

        void t_char_source:: unget( token &tkn ) throw()
        {
            while(tkn.size)
            {
                push_front( tkn.pop_back() );
            }
        }

        void t_char_source:: uncpy( const token &tkn )
        {
            token tmp(tkn);
            unget(tmp);
        }

        void t_char_source:: unget( const char C )
        {
            unget( new t_char(C) );
        }

        void t_char_source:: unget( const string &s )
        {
            token tkn(s);
            unget(tkn);
        }

        void t_char_source:: unget( const char *s )
        {
            token tkn(s);
            unget(tkn);
        }

    }

}



