#ifndef YOCTO_LINGUA_XGEN_INCLUDED
#define YOCTO_LINGUA_XGEN_INCLUDED 1

#include "yocto/lingua/parser.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class xgen
            {
            public:
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

                lingua::parser *generate(xnode *tree,const bool output_files);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
