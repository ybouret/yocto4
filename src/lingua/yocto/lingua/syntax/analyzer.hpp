#ifndef YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED 1

#include "yocto/lingua/syntax/grammar.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            class analyzer
            {
            public:
                virtual ~analyzer() throw();
                explicit analyzer(const grammar &g);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(analyzer);
            };
        }
    }
}

#endif

