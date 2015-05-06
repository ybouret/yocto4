#include "yocto/lang/lexical/plugin/rstring.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            rstring:: ~rstring() throw()
            {

            }

            rstring:: rstring(const char   *id, lexer &parent) :
            plugin(id,parent),content()
            {

                back("'",         this, & rstring::on_quit);
                make("CHAR", ".", this, & rstring::gather);
            }

            void rstring:: on_call(const token &)
            {
                content.clear();
            }

            void rstring:: on_quit(const token &)
            {
                lex->unget(*this, content);
            }

            pattern * rstring:: trigger() const { return single::create('\''); }

            bool rstring:: gather(const token &tkn )
            {
                assert(1==tkn.size);
                content.append(char(tkn.head->code));
                return false;
            }

        }

    }
}
