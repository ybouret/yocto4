#ifndef YOCTO_LINGUA_SYNTAX_COMPOSITE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_COMPOSITE_INCLUDED 1

#include "yocto/lingua/syntax/rules.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
            class composite : public rule
            {
            public:
                virtual ~composite() throw();
                
                void add( rule &r );
                
            protected:
                explicit composite(const string &id );
                vector<rule::ptr> items;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(composite);
            };
        }
    }
    
    
}

#endif
