#include "yocto/lang/syntax/parse-node.hpp"
#include <cstring>

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            lexeme * & parse_node:: __lex() throw()
            {
                return *(lexeme **) &wksp[0];
            }
            
            
            parse_node:: ~parse_node() throw()
            {
                if( terminal )
                {
                    lexeme * lx = __lex();
                    if( lx ) 
                        delete lx;
                }
                else
                {
                    child_list &ch = children();
                    while( ch.size ) delete ch.pop_back();
                }
                memset(wksp,0,sizeof(wksp));
            }
            
            
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
            parse_node:: parse_node( const string &label_ref, lexeme *lx ) throw() :
            label( label_ref ), 
            terminal( lx != 0 ),
            prev(0),
            next(0), 
            parent(0),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));
                if( lx )
                {
                    __lex() = lx;
                }
            }
            
            lexeme * parse_node:: lex() throw()
            {
                assert(terminal);
                return __lex();
            }
            
            parse_node::child_list & parse_node::children() throw()
            {
                assert(!terminal);
                return *(child_list *) &wksp[0];
            }
            
            const lexeme * parse_node:: lex() const throw()
            {
                assert(terminal);
                return *(lexeme **) &wksp[0];
            }
            
            const parse_node::child_list & parse_node::children() const throw()
            {
                assert(!terminal);
                return *(child_list *) &wksp[0];
            }
            
            
            void parse_node:: restore( lexer &Lxr, parse_node *node ) throw()
            {
                assert(node!=NULL);
                if( node->terminal )
                {
                    lexeme * &lx = node->__lex();
                    assert( lx != NULL );
                    Lxr.unget(lx);
                    lx = 0;
                }
                else 
                {
                    child_list &ch = node->children();
                    while( ch.size ) 
                        restore(Lxr,ch.pop_back());
                }
                delete node;
            }
            
			parse_node *parse_node:: create( const string &label_ref, lexeme *lx )
			{
				assert(0!=lx);
				try 
				{
					return new parse_node(label_ref,lx);
				}
				catch(...)
				{
                    delete lx;
					throw;
				}
			}
            
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
                fp.append("rankdir=TB;");
                viz(fp);
                fp.append("}\n");
            }
            
            
        }
        
    }
}