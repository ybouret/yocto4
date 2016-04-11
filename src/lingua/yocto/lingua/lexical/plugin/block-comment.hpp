#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_BLOCK_COMMENT_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_BLOCK_COMMENT_INCLUDED 1

#include "yocto/lingua/lexical/plugin.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            class block_comment : public plugin
            {
            public:
                virtual ~block_comment() throw();
                explicit block_comment(const string &id,
                                       lexer        &lxr,
                                       const string &callExpr,
                                       const string &backExpr);
                virtual void on_call(const token &);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(block_comment);

            };

        }

    }

}

#endif

