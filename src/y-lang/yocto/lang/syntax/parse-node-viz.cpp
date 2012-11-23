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
                
                fp << " [ label=\"'" << label << "'\"];\n";
                // link it to its parent
                if( parent )
                {
                    assert( ! parent->terminal );
                    const unsigned u = parent->children().index_of(this);
                    regex::show_tag(fp, parent); fp<<" -> "; regex::show_tag(fp,this);
                    fp(" [label=\"%u\"];\n",u);
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
                    if( lx->size )
                    {
                        regex::show_tag(fp, lx);
                        fp<<" [ label=\"'";
                        const string s = lx->to_string();
                        for( size_t i=0; i < s.size(); ++i )
                        {
                            char C = s[i];
                            
                            switch( C )
                            {
                                case '"':
                                    fp.write('\\');
                                default:
                                    fp.write(C);
                            };
                        }
                        
                        fp<<"'\", shape=house];\n";
                        regex::show_tag(fp,lx); fp<<" -> "; regex::show_tag(fp,this); fp << " [arrowhead=box];\n";
                    }
                }
            }
            
            void  parse_node:: graphviz( const string &id, ios::ostream &fp ) const
            {
                fp << "digraph " << id << "{\n";
                fp << "rankdir=TB;\n";
                fp << "ordering=out;\n";
                viz(fp);
                fp << "}\n";
            }
            
            void  parse_node:: graphviz( const char   *id, ios::ostream &fp ) const
            {
                const string ID(id);
                graphviz(ID,fp);
            }
            
        }
    }
    
}
