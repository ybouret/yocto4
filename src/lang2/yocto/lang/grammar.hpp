#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/lexical/rule.hpp"

namespace yocto
{
    namespace lang
    {

        class grammar : public object
        {
        public:
            explicit grammar(const string &id);
            virtual ~grammar() throw();

            const string name;


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGM(grammar);

        };

    }
    
}

#endif

