#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            void parse_node:: viz(ios::ostream &fp) const 
            {
                // declare the node
                regex::show_tag(fp,this); 
				fp.append(" [ label=\"'");
				fp.append(label);
				fp.append("'\"];\n");
                
                // link it to its parent
                if( parent )
                {
                    regex::show_tag(fp, parent); fp.append(" -> "); regex::show_tag(fp,this); fp.append(";\n");
                }
                if( !terminal )
                {
                    const child_list &ch = children();
                    for( const parse_node *node = ch.head; node; node=node->next )
                    {
                        node->viz(fp);
                    }
                    
                }
                else 
                {
                    const lexeme *lx = lex();
                    regex::show_tag(fp, lx);
                    fp.append(" [ label=\"'");
                    const string s = lx->to_string();
                    fp.append(s);
                    fp.append("'\", shape=house];\n");
                    regex::show_tag(fp,lx); fp.append(" -> "); regex::show_tag(fp,this); fp.append(" [arrowhead=box];\n");
                }
            }
            
            void  parse_node:: graphviz( const string &id, ios::ostream &fp ) const
            {
                fp.append("digraph "); fp.append(id); fp.append(" {\n");
                fp.append("rankdir=TB;\n");
                fp.append("ordering=out;\n");
                viz(fp);
                fp.append("}\n");
            }
            
            void  parse_node:: graphviz( const char   *id, ios::ostream &fp ) const
            {
                const string ID(id);
                graphviz(ID,fp);
            }

        }
    }

}
