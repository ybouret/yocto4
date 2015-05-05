#ifndef YOCTO_LANG_SYNTAX_LANGEN_INCLUDED
#define YOCTO_LANG_SYNTAX_LANGEN_INCLUDED 1

#include "yocto/lang/parser.hpp"
#include "yocto/lang/pattern/dict.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class LanGen
            {
            public:
                typedef intr_ptr<string,aggregate> agg_ptr;
                typedef set<string,agg_ptr>        agg_set;

                typedef intr_ptr<string,terminal>  term_ptr;
                typedef set<string,term_ptr>       term_set;

                const xnode      *root;
                auto_ptr<parser>  P;
                agg_set           rules;
                term_set          terms;

                LanGen( const xnode *node );
                ~LanGen() throw();

            private:
                void collect(const xnode *node);
                void populate();
                void find_rules_from(const xnode *node);
                void build_rule_from(const xnode *node);

                YOCTO_DISABLE_COPY_AND_ASSIGN(LanGen);
            };
        }
    }

}

#endif

