#ifndef YOCTO_LANG_SYNTAX_LANGEN_INCLUDED
#define YOCTO_LANG_SYNTAX_LANGEN_INCLUDED 1

#include "yocto/lang/syntax/grammar.hpp"

#include "yocto/lang/pattern/dict.hpp"

#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"
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
                typedef set<string,pRule>     RDict;

                const xnode      *root;
                RDict             rules;
                p_dict            terms;
                auto_ptr<grammar> G;

                LanGen( const xnode *node );
                ~LanGen() throw();

            private:
                void collect(const xnode *node);
                void populate();
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(LanGen);
            };
        }
    }

}

#endif

