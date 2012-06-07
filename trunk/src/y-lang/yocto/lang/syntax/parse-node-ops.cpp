#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            void parse_node:: AST() throw()
            {
                if( terminal != 0 )
                {
                    if( flags == syntax::is_specialized )
                    {
                        lex()->release();
                    }
                }
                else
                {
                    child_list &chl = children();
                    child_list  tmp;
                    
                    //==========================================================
                    // Pass 1: remove discardable nodes
                    //==========================================================
                    while( chl.size > 0 )
                    {
                        parse_node *node = chl.pop_front();
                        if( node->flags == syntax::is_discardable )
                        {
                            delete node;
                        }
                        else 
                        {
                            node->AST();
                            tmp.push_back(node);
                        }
                    }
                    
                    //==========================================================
                    // Pass 2: merge possible nodes
                    //==========================================================
                    while( tmp.size > 0 )
                    {
                        parse_node *node = tmp.pop_front();
                        if( node->flags == syntax::is_merging )
                        {
                            assert(node->terminal==0);
                            child_list &sub = node->children();
                            while( sub.size )
                            {
                                parse_node *child = sub.pop_front();
                                child->parent = this;
                                chl.push_back(child);
                            }
                            delete node;
                        }
                        else
                            chl.push_back(node);
                    }
                }
            }
            
            void  parse_node:: out( ios::ostream &fp, size_t &depth ) const
            {
                for( size_t i=0; i < depth; ++i ) fp.write(' ');
                fp.write('<');
                fp.append(label);
                fp.write('>');
                if( terminal )
                {
                    if( lex()->size )
                    {
                        fp.write('\t');
                        fp.write('\'');
                        const string s = lex()->to_string();
                        fp.append(s);
                        fp.write('\'');
                    }
                    fp.write('\n');
                }
                else 
                {
                    fp.write('\n');
                    ++depth;
                    const child_list &chl = children();
                    for( const parse_node *node = chl.head; node; node=node->next )
                    {
                        node->out(fp,depth);
                    }
                    --depth;
                }
            }

            void  parse_node:: output( ios::ostream &fp ) const
            {
                size_t depth = 0;
                out(fp,depth);
            }
        }
        
    }
    
}
