#ifndef YOCTO_LANG_AST_INCLUDED
#define YOCTO_LANG_AST_INCLUDED 1

#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        class AST : public object 
        {
        public:
            typedef core::list_of<AST> SubTree;
            
            explicit AST(  const syntax::parse_node &node );
            virtual ~AST() throw();
            const string &label; //!< reference to the original rule label
            AST *         next;
            AST *         prev;
            const bool    terminal; //!< same a node
            
            string        &content() throw();
            const string  &content() const throw();
            SubTree       &subtree() throw();
            const SubTree &subtree() const throw();
            
        private:
            static const size_t max_size = sizeof(string) > sizeof(SubTree) ? sizeof(string) : sizeof(SubTree);
            uint64_t wksp[ YOCTO_U64_FOR_SIZE(max_size) ];
            YOCTO_DISABLE_COPY_AND_ASSIGN(AST);
        };
        
    }
}

#endif
