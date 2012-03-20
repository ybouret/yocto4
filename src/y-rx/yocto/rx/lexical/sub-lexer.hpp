#ifndef YOCTO_RX_SUB_LEXER_INCLUDED
#define YOCTO_RX_SUB_LEXER_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
    namespace regex
    {
        namespace lexical
        {
         
            class sub_lexer : public object, public counted
            {
            public:
                explicit sub_lexer( const string &id );
                virtual ~sub_lexer() throw();
                
                const string   name;
                const string & key() const throw();
                
                void make( pattern *p, const action &a );
                
                
                
            private:
                rules rules_;
                YOCTO_DISABLE_COPY_AND_ASSIGN(sub_lexer);
            };
            
        }
    }
}
#endif
