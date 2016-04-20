#ifndef YOCTO_LINGUA_XGEN_INCLUDED
#define YOCTO_LINGUA_XGEN_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/alias.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/hashing/mph.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class xgen
            {
            public:

                //! internal parser to produce grammar AST
                class Parser : public lingua::parser
                {
                public:
                    explicit Parser(const bool output_files);
                    virtual ~Parser() throw();
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(Parser);
                };


                xgen();
                ~xgen() throw();
                
                //! top level generation
                lingua::parser *generate(xnode *tree,const bool output_files);

                typedef alias_ptr<string,rule>       rule_ptr;
                typedef set<string,rule_ptr>         rules_db;
                
                auto_ptr<lingua::parser> xprs;
                rules_db                 rules;

                hashing::mperf           htop; //!< RULE,LXR
                hashing::mperf           hsub; //!< ID,RXP,RAW
                hashing::mperf           hmod; //!< +,*,?
                hashing::mperf           hres; //!< reserved words: drop, endl, comment
                int                      icom; //!< index for comment
                
                void create_leading_rule(const xnode *top);
                void create_lexical_rule(const xnode *top);
                
                void        initialize( xlist &top_level ); //!< initialize plugin and remove their lexical rules
                void        load_plugin(const string &id, xnode *node);
                void        ld_cstring(const string &id, const xlist &args);
                void        ld_rstring(const string &id, const xlist &args);

                rule       &fetch_agg(const string &label); //!< return existing rule or new rule as aggregate
                rule       &fetch_rxp(const string &label);
                rule       &fetch_raw(const string &label);



                //! grow parent from child node
                void grow(compound &parent, const xnode *node);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
