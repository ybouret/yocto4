#ifndef YOCTO_LANG_XGEN_INCLUDED
#define YOCTO_LANG_XGEN_INCLUDED 1


#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ptr/alias.hpp"
#include "yocto/hashing/perfect.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            class xgen
            {
            public:
                typedef alias_ptr<string,aggregate> agg_ptr;
                typedef set<string,agg_ptr>         agg_set;


                const xnode      *root;
                auto_ptr<parser>  xprs;
                const char       *name;
                agg_set           agg;
                
                hashing::perfect  cmph; //!< lxr codes: @drop, @endl, @comment
                hashing::perfect  mmph; //!< lxr meta: RXP|RAW

                unsigned          indx; //!< for sub rules naming
                unsigned          jndx; //!< for lexical rules naming



                xgen(const xnode *node);
                ~xgen() throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);

                void     process_rule_level1(const xnode *node);
                void     process_lxr__level1(const xnode *node);
                pattern *compile_lexical(const xnode *node);
            };
        }
    }
}

#endif

