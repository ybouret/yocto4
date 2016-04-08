#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

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
curr(0), \
hist(),\
root( new lexical::scanner(root_id,line) ), \
scdb(2,as_capacity),                        \
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
            if(!scdb.insert(root))
            {
                throw exception("{%s}: failure to store root scanner <%s>", name.c_str(), root->name.c_str());
            }
            restart();
        }


        void lexer:: restart() throw()
        {
            line = 1;
            hist.clear();
            curr = root.__get(); assert(curr);
        }


        lexical::scanner & lexer:: declare(const string &id)
        {
            lexical::scanner::ptr p( new lexical::scanner(id,line) );
            if( !scdb.insert(p) )
            {
                throw exception("{%s}: multipler scanner <%s>", name.c_str(), id.c_str());
            }
            return *p;
        }

    }

}

