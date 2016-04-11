#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_END_OF_LINE_COMMENT_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_END_OF_LINE_COMMENT_INCLUDED 1

#include "yocto/lingua/lexical/plugin.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            class end_of_line_comment : public plugin
            {
            public:
                virtual ~end_of_line_comment() throw();
                explicit end_of_line_comment(const string &id,
                                             lexer        &lxr,
                                             const string &expr);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(end_of_line_comment);
                virtual void on_call(const token &);
                
            };

        }

    }
}


#endif
