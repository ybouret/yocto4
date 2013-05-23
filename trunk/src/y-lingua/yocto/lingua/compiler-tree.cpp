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
        
        syntax::xnode *compiler::rewrite( syntax::xnode *node )
        {
            assert(node);
            grammar      &G = *this;
            const string &atoms_label = G["ATOMS"].label;
            
            if( ! node->terminal )
            {
                syntax::xnode::child_list  target;
                syntax::xnode::child_list &source = node->children();
                
                //! recursive call
                while(source.size)
                {
                    target.push_back( rewrite(source.pop_front()) );
                }
                
                assert(target.size>0);
                if( "ALT" == target.tail->label )
                {
                    std::cerr << "Effective Rewrite with #target=" << target.size << std::endl;
                    //----------------------------------------------------------
                    // rewriting
                    //----------------------------------------------------------
                    assert(0==source.size);
                    source.push_front( target.pop_back() );
                    syntax::xnode *alt = source.tail;
                    assert(target.size>0);
                    if( target.size > 1 )
                    {
                        syntax::xnode *atoms = syntax::xnode::create( atoms_label, 0, syntax::is_merging_one);
                        atoms->parent = alt;
                        alt->children().push_front(atoms);
                        __claim(atoms, target);
                    }
                    else
                    {
                        __claim(alt,target);
                    }
                }
                else
                    source.swap_with(target);
            }
            
            return node;
        }
        
    }
    
}

