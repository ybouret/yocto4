#ifndef YOCTO_LANG_SYNTAX_LANGEN_INCLUDED
#define YOCTO_LANG_SYNTAX_LANGEN_INCLUDED 1

#include "yocto/lang/parser.hpp"
#include "yocto/lang/pattern/dict.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/hashing/perfect.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class LanGen
            {
            public:
                typedef intr_ptr<string,aggregate> rule_ptr;
                typedef set<string,rule_ptr>       rule_set;

                typedef intr_ptr<string,terminal>  term_ptr;
                typedef set<string,term_ptr>       term_set;

                const xnode      *root;
                auto_ptr<parser>  P;
                rule_set          rules;
                term_set          rxp;
                term_set          raw;
                hashing::perfect  cmph; //!< collect hasher
                hashing::perfect  rmph; //!< growing rules hasher
                hashing::perfect  jmph; //!< for kind of joker
                
                LanGen( const xnode *node );
                ~LanGen() throw();

                

            private:
                void collect(const xnode *node);
                void find_rules_from(const xnode *node);
                void build_rule_from(const xnode *node);

                void grow_rule(logical *parent, const xnode *sub);

                logical &get_std(const xnode *node);
                rule    &get_rxp(const xnode *node);
                rule    &get_raw(const xnode *node);

                YOCTO_DISABLE_COPY_AND_ASSIGN(LanGen);
                unsigned indx;

                void grow_sub( logical *parent, const xnode *node);
                void grow_alt( logical *parent, const xnode *node);
                void grow_itm( logical *parent, const xnode *node);

                void simplify(rule *r) throw();
                
            };
        }
    }

}

#endif

