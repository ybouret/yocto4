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

                typedef alias_ptr<string,aggregate> agg_ptr;
                typedef set<string,agg_ptr>         aggDB;
                typedef alias_ptr<string,rule>      term_ptr;
                typedef set<string,term_ptr>        termDB;
                
                auto_ptr<lingua::parser> xprs;
                aggDB                    agg_db;
                termDB                   rxp_db;
                termDB                   raw_db;
                hashing::mperf           htop; //!< RULE,LXR
                hashing::mperf           hsub; //!< ID,RXP,RAW

                
                void create_rule(const xnode *top);
                void create_lxr_(const xnode *top);
                
                
                aggregate & fetch_agg(const string &label);
                rule      & fetch_rxp(const string &label);
                rule      & fetch_raw(const string &label);

                //! grow parent from child node
                void grow(compound &parent, const xnode *node);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
