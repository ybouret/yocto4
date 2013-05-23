#include "yocto/lingua/compiler.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        static inline
        void __claim( syntax::xnode *parent, syntax::xnode::child_list &ch ) throw()
        {
            while(ch.size)
            {
                syntax::xnode *node = ch.pop_back();
                node->parent = parent;
                parent->children().push_front(node);
            }
        }
        
        syntax::xnode *compiler::rewrite( syntax::xnode *root )
        {
            assert(root);
            if( ! root->terminal )
            {
                syntax::xnode::child_list &children = root->children();
                assert(children.size>0);
                syntax::xnode::child_list  target;
                
                //--------------------------------------------------------------
                //-- recursive call
                //--------------------------------------------------------------
                while(children.size) target.push_back( rewrite(children.pop_front()) );
                target.swap_with(children);
                
                //--------------------------------------------------------------
                //-- analysis
                //--------------------------------------------------------------
                assert(children.size>0);
                while(children.size)
                {
                    syntax::xnode *node = children.pop_front();
                    if( node->label == "ATOMS" )
                    {
                        syntax::xnode::child_list &ch = node->children();
                        if( ch.tail->label == "ALT" )
                        {
                            syntax::xnode *alt = ch.pop_back();
                            alt->children().push_front(node);
                            node->parent = alt;
                            alt->parent = root;
                            target.push_back(alt);
                            continue;
                        }
                    }
                    
                    target.push_back(node);
                }
                target.swap_with(children);
                
            }
            return root;
        }
        
    }
    
}

