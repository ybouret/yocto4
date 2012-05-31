#ifndef YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED
#define YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED 1

#include "yocto/lang/lexer.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            class parse_node : public object
            {
            public:
                typedef core::list_of<parse_node> child_list;
                const string &label; //!< reference to the corresponding rule label
                parse_node   *prev;
                parse_node   *next;
                parse_node   *parent;
                child_list    children;
                
                virtual ~parse_node() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(parse_node);
            };
        }
    }
    
}


#endif
