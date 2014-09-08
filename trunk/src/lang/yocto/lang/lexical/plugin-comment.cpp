#include "yocto/lang/lexer.hpp"
#include "yocto/lang/lexical/plugin-comment.hpp"

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
                make("CHAR", posix::dot(), this, & scanner::discard);
                back(posix::endl(),        this, & scanner::newline);
            }
            
            pattern * comment:: trigger() const
            {
                return compile(regexp,dict);
            }
            
            void comment:: enter(const token &)
            {
                // do nothing
                std::cerr << "entering comment '" << regexp << "' @line " << line << std::endl;
            }
        }
    }
}
