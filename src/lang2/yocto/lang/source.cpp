#include "yocto/lang/source.hpp"

namespace yocto
{
    namespace lang
    {

        source::  source() throw() : token() {}
        source:: ~source() throw() {}

        t_char *source:: get( ios::istream &fp )
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

        const t_char * source:: peek( ios::istream &fp )
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


        void source:: skip(size_t n) throw()
        {
            assert(n<=size);
            while(n-->0)
            {
                delete pop_front();
            }
        }

        void source:: unget( t_char *ch ) throw()
        {
            assert(ch);
            push_front(ch);
        }

        void source:: unget( token &tkn ) throw()
        {
            while(tkn.size)
            {
                push_front( tkn.pop_back() );
            }
        }

        void source:: uncpy( const token &tkn )
        {
            token tmp(tkn);
            unget(tmp);
        }

        void source:: unget( const char C )
        {
            unget( new t_char(C) );
        }

        void source:: unget( const string &s )
        {
            token tkn(s);
            unget(tkn);
        }

        void source:: unget( const char *s )
        {
            token tkn(s);
            unget(tkn);
        }

    }

}



