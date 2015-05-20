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
                const char       *name;
                
                xgen(const xnode *node);
                ~xgen() throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);

                void process_rule_level1(const xnode *node);
                void process_lxr__level1(const xnode *node);
            };
        }
    }
}

#endif

