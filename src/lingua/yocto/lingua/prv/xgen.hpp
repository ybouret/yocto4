#ifndef YOCTO_LINGUA_XGEN_INCLUDED
#define YOCTO_LINGUA_XGEN_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ptr/alias.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/hashing/mph.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

#define YXGEN_OUT(OUTPUT) do { if(verbose) { std::cerr << "[XGEN] " << OUTPUT << std::endl; } } while(false)
            
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


                xgen(const bool verb);
                ~xgen() throw();
                
                //! top level generation
                lingua::parser *generate(xnode *tree);

                typedef alias_ptr<string,rule>       rule_ptr;
                typedef set<string,rule_ptr>         rules_db;
                
                auto_ptr<lingua::parser> xprs;
                rules_db                 rules;

                hashing::mperf           htop;    //!< RULE,LXR
                hashing::mperf           hsub;    //!< ID,RXP,RAW
                hashing::mperf           hmod;    //!< +,*,?
                hashing::mperf           hres;    //!< reserved words: drop, endl, comment
                int                      icom;    //!< index for comment
                const bool               verbose;
                sorted_vector<string>    no_single;

                void create_leading_rule(const xnode *top);
                void create_lexical_rule(const xnode *top);
                
                void        initialize( xlist &top_level ); //!< initialize plugin and remove their lexical rules
                void        load_plugin(const string &id, xnode *node);
                void        ld_cstring(const string &id, const xlist &args);
                void        ld_rstring(const string &id, const xlist &args);
                bool        is_alias(const xnode *node); //!< detect is a rule is an alias

                rule       &fetch_agg(const string &label); //!< return existing rule or new rule as aggregate
                rule       &fetch_rxp(const string &label); //!< return existing or new regexp terminal
                rule       &fetch_raw(const string &label); //!< return existing or new raw    terminal

                void        check_semantic(const string &id, const xnode *node);

                //! grow parent from child node
                void grow(compound &parent, const xnode *node);


                //! optimize
                /**
                 - univocal to jettison syntax only terminals
                 - jettison rule when it is single alternation
                 */
                void optimize();

                static bool is_internal(const rule &r) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
