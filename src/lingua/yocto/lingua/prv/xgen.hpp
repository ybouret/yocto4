#ifndef YOCTO_LINGUA_XGEN_INCLUDED
#define YOCTO_LINGUA_XGEN_INCLUDED 1

#include "yocto/lingua/parser.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            class xgen : public lingua::parser
            {
            public:
                const bool output;

                explicit xgen(const bool output_files);
                virtual ~xgen() throw();

                static xnode *rewrite(xnode *tree);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };

        }

    }
}

#endif
