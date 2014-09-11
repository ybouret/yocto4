#include "yocto/lang/lexer.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"

#include "yocto/lang/pattern/posix.hpp"


namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            comment:: ~comment() throw()
            {
            }
            
            comment:: comment(const string &id,
                              lexer        &parent,
                              const string &expr) :
            plugin(id,parent),
            regexp(expr)
            {
                setup();
            }
            
            comment:: comment(const char  * id,
                              lexer       & parent,
                              const char  * expr) :
            plugin(id,parent),
            regexp(expr)
            {
                setup();
            }
            
            pattern * comment:: trigger() const
            {
                return compile(regexp,dict);
            }
            
            void comment:: on_call(const token &)
            {
                // do nothing
            }
            
            void comment:: setup()
            {
                make("CHAR", posix::dot(), this, & scanner::discard);
                back(posix::endl(),        this, & scanner::newline);
            }
            
        }
    }
}
