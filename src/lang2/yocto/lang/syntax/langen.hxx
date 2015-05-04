#ifndef YOCTO_LANG_SYNTAX_LANGEN_INCLUDED
#define YOCTO_LANG_SYNTAX_LANGEN_INCLUDED 1

#include "yocto/lang/parser.hpp"
#include "yocto/lang/pattern/dict.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class LanGen
            {
            public:
                typedef intr_ptr<string,rule> pRule;

                const xnode      *root;
                auto_ptr<parser>  P;

                LanGen( const xnode *node );
                ~LanGen() throw();

            private:
                void collect(const xnode *node);

                YOCTO_DISABLE_COPY_AND_ASSIGN(LanGen);
            };
        }
    }

}

#endif

