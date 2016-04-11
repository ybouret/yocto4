#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"
#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            end_of_line_comment:: ~end_of_line_comment() throw()
            {
            }

            end_of_line_comment:: end_of_line_comment(const string &id,
                                                      lexer        &lxr,
                                                      const string &expr) :
            plugin(id,lxr,expr)
            {
                back("[:endl:]", this, &scanner::endl_cb);
                const string label = id + "_any1";
                const action forget(this, &scanner::discard);
                make(label, any1::create(), forget);
            }

            void end_of_line_comment::on_call(const token &)
            {
            }


        }

    }

}