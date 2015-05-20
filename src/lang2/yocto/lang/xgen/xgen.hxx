#ifndef YOCTO_LANG_XGEN_INCLUDED
#define YOCTO_LANG_XGEN_INCLUDED 1


#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ptr/alias.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            class xgen
            {
            public:
                const xnode      *root;
                auto_ptr<parser>  xprs;
                
                xgen(const xnode *node);
                ~xgen() throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);
            };
        }
    }
}

#endif

