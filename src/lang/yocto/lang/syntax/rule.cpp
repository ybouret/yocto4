#include "yocto/lang/syntax/rule.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            rule:: ~rule() throw() {}
            
            rule:: rule( const string &id ) :
            label(id),
            next(0),
            prev(0)
            {
                
            }
            
            
            void rule:: grow( xtree &Tree, xtree &Node) throw()
            {
                if(Node)
                {
                    if(!Tree)
                    {
                        Tree = Node;
                    }
                    else
                    {
                        assert(!Tree->terminal);
                        Tree->add(Node);
                        Node->parent = Tree;
                    }
                    Node = NULL;
                }
            }
            
            void rule:: check( const xtree Tree ) const
            {
                if( Tree && Tree->terminal)
                {
                    throw exception("syntax.rule '%s': unexpected tree=terminal '%s'!!", label.c_str(), Tree->label.c_str());
                }
            }

        }
        
    }
    
}
