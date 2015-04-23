#ifndef YOCTO_LANG_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LANG_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            class scanner : public counted_object
            {
            public:
                virtual ~scanner() throw();
                explicit scanner(const string &id);

                const string name;
                r_list       rules;
                l_list       cache;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);


            };
        }
        
    }
}



#endif

