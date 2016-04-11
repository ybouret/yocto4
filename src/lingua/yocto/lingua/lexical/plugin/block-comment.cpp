#include "yocto/lingua/lexical/plugin/block-comment.hpp"
#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            block_comment:: ~block_comment() throw()
            {
            }

            block_comment::block_comment(const string &id,
                                         lexer        &lxr,
                                         const string &callExpr,
                                         const string &backExpr) :
            yocto::lingua::lexical::plugin(id,lxr,callExpr)
            {
                // return
                back(backExpr, this, &scanner::drop_cb);


                // count line
                {
                    const string label = id + "_endl";
                    endl(label);
                }

                // discard other
                {
                    const string label = id+"_any1";
                    const action forget(this, &scanner::discard);
                    make(label, any1::create(), forget);
                }
            }


            void block_comment:: on_call(const token &)
            {
            }
        }
        
    }
    
}
