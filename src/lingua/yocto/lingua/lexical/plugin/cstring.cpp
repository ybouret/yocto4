#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/lexer.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            cstring:: ~cstring() throw()
            {
            }


            cstring:: cstring(const string &id, lexer &lxr) :
            plugin(id,lxr,"\""),
	    content()
            {
                back(trigger, this, &cstring::on_back);


                {
                    const string label = id + "+";
                    make(label,"[\\x00-\\xff]",this,&cstring::append);
                }


            }

            
            void cstring:: on_call(const token &)
            {
                content.clear();
            }

            void cstring:: on_back(const token &)
            {
                lex->unget(*this,line,content);
            }

            bool cstring:: append(const token &t)
            {
                for(const t_char *ch = t.head;ch;ch=ch->next)
                {
                    content.append(char(ch->code));
                }
                return false;
            }

        }
    }
}
