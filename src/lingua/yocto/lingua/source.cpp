#include "yocto/lingua/source.hpp"
namespace yocto
{
    namespace lingua
    {
        source:: ~source() throw()
        {

        }


        source:: source() throw() :
        input(0),
        cache(),
        attached()
        {
        }

        source:: source(ios::istream &fp) throw() :
        input(&fp),
        cache(),
        attached()
        {
        }
        

        t_char * source:: query()
        {
            return cache.size ? cache.pop_front() : query_from_input();
        }

        void source:: store(t_char *ch) throw()
        {
            assert(ch);
            cache.push_front(ch);
        }

        bool source:: is_active()
        {
            t_char *ch = query();
            if(ch)
            {
                store(ch);
                return true;
            }
            else
            {
                return false;
            }
        }

        void source::unread(token  &tkn) throw()
        {
            while(tkn.size)
            {
                cache.push_front( tkn.pop_back() );
            }
        }

        void source:: unread_a_copy_of(const token &tkn)
        {
            for(const t_char *ch=tkn.tail;ch;ch=ch->prev)
            {
                cache.push_front( new t_char(ch->code) );
            }
        }

        t_char * source:: query_from_input()
        {
            char C = 0;
            while(input)
            {
                // test current input
                if(input->query(C))
                {
                    return new t_char(C);
                }

                // End of input
                input = NULL;

                // still more input ?
                if(attached.size)
                {
                    input = attached.head->addr;
                    delete attached.pop_front();
                }
            }
            // really the end..
            return NULL;
        }

        void source:: attach(ios::istream &fp)
        {
            if(input)
            {
                attached.append(input);
            }
            input = &fp;
        }

        const t_char * source::peek()
        {
            t_char *ans = query();
            if(ans)
            {
                store(ans);
            }
            return ans;
        }

        size_t source:: read() const throw()
        {
            return cache.size;
        }


        void source::skip(size_t n) throw()
        {
            assert(n<=cache.size);
            while(n-->0)
            {
                delete cache.pop_front();
            }
        }

    }
}

