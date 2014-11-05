#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        
        generator:: ~generator() throw()
        {
        }
        
        generator:: generator() :
        parser("generator","scanning")
        {
            
            
            //__________________________________________________________________
            //
            // Generator Grammar
            //__________________________________________________________________
            Terminal  &ID     = terminal("ID","@?[_[:alpha:]][:word:]*");
            Terminal  &COLON  = jettison("separator",':');
            Terminal  &STOP   = jettison("end of rule",';');
            Rule      &EXPR   = plug_term(new lexical::cstring("STRING",*this) );
            Terminal  &PIPE   = jettison("alternate",'|');
            Terminal  &CHAR   = terminal("CHAR","'.'");
            Aggregate &RULE   = assemble("RULE");
            
            
            
            //__________________________________________________________________
            //
            // Make rules
            //__________________________________________________________________
            RULE += ID;
            RULE += COLON;
            
            
            {
                Aggregate   & GROUP = merge();
                Alternate   & CORE = alt();
                CORE |= ID;
                CORE |= EXPR;
                CORE |= CHAR;
                CORE |= GROUP;
                Aggregate   & JOKER = assemble("JOKER", CORE, terminal("ATTR", "[+?*]") );
                Alternate   & ATOM  = choose(JOKER,CORE);
                Aggregate   & ATOMS = agg("ATOMS", syntax::is_merging_one);
                ATOMS += one_or_more(ATOM);
                ATOMS += zero_or_more(assemble("OR",PIPE,ATOMS));
                
                GROUP += jettison("left paren",  '(');
                GROUP += ATOMS;
                GROUP += jettison("right paren", ')');
                
                RULE += ATOMS;
                
            }
            
            RULE += STOP;
            set_root( zero_or_more(RULE,"RULES") );
            
            //__________________________________________________________________
            //
            // Lexical
            //__________________________________________________________________
            plug_meta( new lexical::comment("comment",*this,"//") );
            scanner.make("blank", "[:blank:]", discard);
            scanner.make("endl" , "[:endl:]",  newline);
            
            
        }
        
        
        static inline
        void check_single_atom_of(syntax::xnode *node)
        {
            syntax::xnode::child_list tmp;
            while(node->count())
            {
                syntax::xnode *child = node->pop();
                if( "ATOMS" == child->label && 1 == child->count())
                {
                    tmp.push_back(child->pop());
                    delete child;
                }
                else
                    tmp.push_back(child);
            }
            while(tmp.size) node->add(tmp.pop_front());
        }
        
        void generator:: rewrite(syntax::xnode *root) const throw()
        {
            assert(root);
            assert("RULES" == root->label );
            reshape(root);
        }
        
        void generator:: reshape(syntax::xnode *node) const throw()
        {
            if(node && !node->terminal)
            {
                XList tmp;
                //______________________________________________________________
                //
                // Pass 1: Recursivity
                //______________________________________________________________
                while(node->count())
                {
                    XNode *child = node->pop();
                    tmp.push_back(child);
                    reshape(child);
                }
                
                //______________________________________________________________
                //
                // Pass 2: Tree rotations
                //______________________________________________________________
                while(tmp.size)
                {
                    XNode *child = tmp.pop_front();
                    if("ATOMS"        == child->label &&
                       child->count() >  1 &&
                       "OR" == child->children().tail->label)
                    {
                        XNode *Or = child->children().pop_back();
                        Or->parent = 0;
                        Or->children().push_front(child);
                        child->parent = Or;
                        node->add(Or);
                        check_single_atom_of(Or);
                    }
                    else
                    {
                        node->add(child);
                    }
                }
                
                //______________________________________________________________
                //
                // Pass 3: fusion
                //______________________________________________________________
                if( "OR" == node->label )
                {
                    while(node->count())
                    {
                        XNode *child = node->pop();
                        if( "OR" == child->label )
                        {
                            while(child->count())
                            {
                                tmp.push_back( child->pop() );
                            }
                            delete child;
                        }
                        else
                            tmp.push_back(child);
                    }
                    while(tmp.size) node->add(tmp.pop_front());
                }
                
                if( "JOKER" == node->label )
                {
                    node->children().reverse();
                }
            }
            
            
            
        }
        
        
        
        
    }
    
}

