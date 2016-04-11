#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {

        lexer:: ~lexer() throw()
        {
        }

#define Y_LEXER_CTOR()                      \
name(id),                                   \
line(0),                                    \
current(0),                                \
stopped(false),                             \
cache(),                                    \
history(),                                  \
base( new lexical::scanner(root_id,line) ), \
scdb(2,as_capacity),                        \
root( *base ),                              \
dict()

        lexer:: lexer(const string &id, const string &root_id) :
        Y_LEXER_CTOR()
        {
            setup();
        }

        lexer:: lexer(const char *id, const char *root_id) :
        Y_LEXER_CTOR()
        {
            setup();
        }

        void lexer:: setup()
        {
            if(!scdb.insert(base))
            {
                throw exception("{%s}: failure to store root scanner <%s>", name.c_str(), base->name.c_str());
            }
            base->link_to(*this);
            restart();
        }


        void lexer:: restart() throw()
        {
            line = 1;
            history.clear();
            current = &root; assert(current);
            stopped = false;
        }


        lexical::scanner & lexer:: declare(const string &id)
        {
            lexical::scanner::ptr p( new lexical::scanner(id,line) );
            if( !scdb.insert(p) )
            {
                throw exception("{%s}: multipler scanner <%s>", name.c_str(), id.c_str());
            }
            p->link_to(*this);
            return *p;
        }

        void lexer:: stop() throw()
        {
            stopped = true;
        }

        void lexer:: call(const string &id)
        {
            assert(current);
            std::cerr << "<" << name << "." << current->name << ">.call(" << id << ")" << std::endl;
            lexical::scanner::ptr *pp = scdb.search(id);
            if(!pp)
            {
                throw exception("<%s.%s>.call(no scanner <%s>)",name.c_str(),current->name.c_str(),id.c_str());
            }
            history.append(current);
            current = pp->__get();
        }

        void lexer:: back()
        {
            assert(current);
            std::cerr << "<" << name << "." << current->name << ">.back" << std::endl;
            if(history.size<=0)
            {
                throw exception("<%s.%s>:back(empty stack)",name.c_str(),current->name.c_str());
            }
            current = history.tail->addr;
            history.remove();
        }

        lexeme * lexer::get(source &src)
        {
            assert(current!=NULL);
        GET_LEXEME:
            if(cache.size>0)
            {
                return cache.pop_front();
            }
            else
            {
                if(stopped)
                {
                    return 0;
                }
                else
                {
                    bool    ctrl = false;
                    lexeme *lx  = current->get(src,ctrl);
                    if(lx)
                    {
                        // got someting
                        assert(false==ctrl);
                        return lx;
                    }
                    else
                    {
                        if(ctrl) goto GET_LEXEME; //! something happened to the lexer
                        return 0;                 //! end of source
                    }
                }
            }

            return NULL;
        }


    }

}

