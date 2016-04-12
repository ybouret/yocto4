#ifndef YOCTO_LINGUA_SYNTAX_GRAMMAR_INCLUDED
#define YOCTO_LINGUA_SYNTAX_GRAMMAR_INCLUDED 1

#include "yocto/lingua/syntax/compound.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class grammar : public object
            {
            public:
                virtual ~grammar() throw();
                explicit grammar(const string &id);
                
                const string name;

                rule      & decl_term(const string &label,const uint32_t flags=0);
                rule      & opt( const rule &r );
                rule      & zero_or_more(const rule &r);
                rule      & one_or_more(const rule &r);
                aggregate & agg(const string &id,const uint32_t flags=0);
                alternate & alt();


                void graphviz(const string &filename) const;
                void top_level(rule &r) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
                void check(const string &label) const;
                r_list   rules;
                unsigned nopt;
                unsigned nalt;
                unsigned nzom;
                unsigned noom;

            };

        }
    }
}


#endif
