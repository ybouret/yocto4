#include "yocto/lang/lexer.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"

#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"


namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            C_comment:: ~C_comment() throw()
            {
            }
            
            C_comment:: C_comment(const string &id,
                                  lexer        &parent) :
            plugin(id,parent)
            {
                back( logical::EQUAL("*/"), this, & C_comment::on_call); // on call does nothing...
                make("CHAR", posix::dot(),  this, & scanner::discard);
                make("ENDL", posix::endl(), this, & scanner::discard_newline);
            }
            
            pattern * C_comment:: trigger() const
            {
                return logical::EQUAL("/*");
            }
            
            void C_comment:: on_call(const token &)
            {
                // do nothing
            }
        }
    }
}
