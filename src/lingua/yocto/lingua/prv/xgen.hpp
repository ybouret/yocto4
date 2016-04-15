#ifndef YOCTO_LINGUA_XGEN_INCLUDED
#define YOCTO_LINGUA_XGEN_INCLUDED 1

#include "yocto/lingua/parser.hpp"
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

                static xnode *rewrite(xnode *tree);

                //! top level generation
                lingua::parser *generate(xnode *tree,const bool output_files);

                typedef alias_ptr<string,aggregate> agg_ptr;
                typedef set<string,agg_ptr>         agg_db;

                void build_syntax_databases(const xnode *node);

                auto_ptr<lingua::parser> xprs;
                agg_db                   adb;
                hashing::mperf           h_db; //!< ID,RAW,RXP

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
