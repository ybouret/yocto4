#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/code/cast.hpp"
#include <cstring>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
#define Y_XNODE_SHOW 0
            
#if defined(Y_XNODE_SHOW) && (Y_XNODE_SHOW==1)
#define Y_XNODE_AST(CODE) do { CODE; } while(false)
            static inline
            void __indent( int depth )
            {
                if(depth)
                {
                    for(int i= (depth) * 2; i>0; --i) std::cerr << ' ';
                }
            }
#else
#define Y_XNODE_AST(CODE)
#endif
            
            
            xnode * xnode:: AST(xnode *node, int &depth) throw()
            {
                if(!node)
                    return 0;
                
                if(node->terminal)
                {
                    //==========================================================
                    // AST terminal node
                    //==========================================================
                    switch(node->property)
                    {
                        case is_specialized:
                            Y_XNODE_AST(__indent(depth); std::cerr << "[XNODE]: Clear    " << node->label << "='" << *(node->lex()) << "'" << std::endl);
                            node->lex()->clear();
                            break;
                            
                        case is_discardable:
                            Y_XNODE_AST(__indent(depth); std::cerr << "[XNODE]: Jettison " << node->label << "='" << *(node->lex()) << "'" << std::endl);
                            delete node;
                            return 0;
                            
                        default:
                            assert(is_regular==node->property);
                            Y_XNODE_AST(__indent(depth); std::cerr << "[XNODE]: Keep     " << node->label << "='" << *(node->lex()) << "'" << std::endl);
                            break;
                    }
                    return node;
                }
                else
                {
                    //==========================================================
                    // AST non terminal node
                    //==========================================================
                    Y_XNODE_AST(__indent(depth); std::cerr << "[XNODE]: " << node->label << " / #children=" << node->children().size << " @input" << std::endl);
                    //----------------------------------------------------------
                    // recursive cleanup
                    //----------------------------------------------------------
                    xnode::child_list &source = node->children();
                    xnode::child_list  target;
                    while( source.size )
                    {
                        ++depth;
                        xnode *sub = xnode::AST( source.pop_front(),depth );
                        --depth;
                        if(sub)
                        {
                            switch( sub->property )
                            {
                                case is_merging_one:
                                    assert(!sub->terminal);
                                    assert(sub->children().size>0); // impossible here;
                                    if( sub->children().size > 1 )
                                    {
                                        target.push_back(sub);
                                        break;
                                    }
                                    assert(1==sub->children().size);
                                    
                                case is_merging_all:
                                    Y_XNODE_AST(__indent(depth+1); std::cerr << sub->label << " ==> " << node->label << std::endl);
                                    assert(!sub->terminal);
                                    //assert(sub->children().size>0);
                                    while( sub->children().size )
                                    {
                                        xnode *ch = sub->children().pop_front();
                                        ch->parent = node;
                                        target.push_back(ch);
                                    }
                                    delete sub;
                                    break;
                                    
                                default:
                                    target.push_back(sub);
                                    break;
                                    
                            }
                        }
                    }
                    source.swap_with(target);
                    Y_XNODE_AST(__indent(depth); std::cerr << "[XNODE]: " << node->label << " / #children=" << node->children().size << " @output" << std::endl);
                    return node;
                }
            }
        }
        
    }
    
}
