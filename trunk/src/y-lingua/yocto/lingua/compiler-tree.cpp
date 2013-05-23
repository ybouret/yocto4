#include "yocto/lingua/compiler.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        syntax::xnode * compiler:: simplify( syntax::xnode *node ) throw()
        {
            assert(node);
            if( !node->terminal )
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list target;
                while(source.size)
                {
                    syntax::xnode *sub = simplify(source.pop_front());
                    if( sub->label == "ATOMS" )
                    {
                        syntax::xnode::child_list &ch = sub->children();
                        assert(ch.size>0);
                        if( ch.size == 1 )
                        {
                            while(ch.size)
                            {
                                syntax::xnode *n = ch.pop_front();
                                n->parent = node;
                                target.push_back(n);
                            }
                            delete sub;
                            continue;
                        }
                    }
                    
                    target.push_back(sub);
                }
                source.swap_with(target);
            }
            return node;

        }
        
        // top down assembling same kind of nodes
        syntax::xnode * compiler:: assemble( const char *label, syntax::xnode *node ) throw()
        {
            assert(node);
            assert(label);
            if( !node->terminal )
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list target;
                const bool is_kind = label == node->label;
                while(source.size)
                {
                    syntax::xnode *sub = assemble(label,source.pop_front());
                    if( is_kind && label == sub->label )
                    {
                        syntax::xnode::child_list &ch = sub->children();
                        while(ch.size)
                        {
                            syntax::xnode *n = ch.pop_front();
                            n->parent = node;
                            target.push_back(n);
                        }
                        delete sub;
                    }
                    else
                        target.push_back(sub);
                }
                target.swap_with(source);
            }
            return node;
        }
        
        //======================================================================
        // contract consecutive groups on the same level
        //======================================================================
        syntax::xnode * compiler::contract_atoms( syntax::xnode *node ) throw()
        {
            assert(node);
            if(!node->terminal)
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list  target;
                while(source.size)
                {
                    syntax::xnode *sub = contract_atoms( source.pop_front() );
                    syntax::xnode *prv = target.tail;
                    if( "ATOMS" == sub->label && prv && "ATOMS" == prv->label )
                    {
                        syntax::xnode::child_list &ch = sub->children();
                        while(ch.size)
                        {
                            syntax::xnode *n = ch.pop_front();
                            prv->children().push_back(n);
                            n->parent = prv;
                        }
                        delete sub;
                    }
                    else
                        target.push_back(sub);
                }
                source.swap_with(target);
            }
            return node;
        }
        
        
        syntax::xnode *compiler::rewrite( syntax::xnode *root ) throw()
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

