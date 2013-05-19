#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            void xnode:: viz(ios::ostream &fp) const
            {
                //--------------------------------------------------------------
                // declare the node
                //--------------------------------------------------------------
                fp.viz(this);
                
                fp << " [ label=\"'" << label << "'\"];\n";
                
                //--------------------------------------------------------------
                // link it to its parent
                //--------------------------------------------------------------
                if( parent )
                {
                    assert( ! parent->terminal );
                    const unsigned u = parent->children().index_of(this);
                    fp.viz(parent); fp<<" -> "; fp.viz(this);
                    fp(" [label=\"%u\"];\n",u);
                }
                
                if( !terminal )
                {
                    const child_list &ch = children();
                    for( const xnode *node = ch.head; node; node=node->next )
                    {
                        node->viz(fp);
                    }
                    
                }
                else
                {
                    const lexeme *lx = lex();
                    if( lx->size )
                    {
                        fp.viz(lx);
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
                        fp.viz(lx); fp<<" -> "; fp.viz(this); fp << " [arrowhead=box];\n";
                    }
                }
            }
          
            void  xnode:: graphviz( const string &id) const
            {
                ios::ocstream fp(id,false);
                fp << "digraph " << "G" << "{\n";
                fp << "rankdir=TB;\n";
                fp << "ordering=out;\n";
                viz(fp);
                fp << "}\n";
            }
            
            void  xnode:: graphviz( const char   *id ) const
            {
                const string ID(id);
                graphviz(ID);
            }
            
            
        }

    }

}
