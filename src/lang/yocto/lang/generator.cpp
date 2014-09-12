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
            Terminal  &ID     = terminal("ID","[_[:alpha:]][:word:]*");
            Terminal  &COLON  = jettison("separator",':');
            Terminal  &STOP   = jettison("end of rule",';');
            Rule      &EXPR   = plug_term(new lexical::cstring("string",*this) );
            Terminal  &PIPE   = jettison("alternate",'|');
            Terminal  &CHAR   = terminal("CHAR","'.'");
            Aggregate &RULE   = assemble("RULE");
            
            RULE += ID;
            RULE += COLON;
            
            
            {
                Aggregate   & GROUP = merge();
                Alternate   & CORE = alt();
                CORE |= ID;
                CORE |= EXPR;
                CORE |= CHAR;
                CORE |= GROUP;
                Aggregate   & JOKER = assemble("JOKER", CORE, terminal("attr", "[+?*]") );
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
        
        
        void generator:: rewrite(syntax::xnode *node) const throw()
        {
            if(node&& !node->terminal)
            {
                XList tmp;
                //______________________________________________________________
                //
                // Recursivity
                //______________________________________________________________
                while(node->count())
                {
                    XNode *child = node->pop();
                    tmp.push_back(child);
                    rewrite(child);
                }
                
                //______________________________________________________________
                //
                // tree rotations
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
                    }
                    else
                    {
                        node->add(child);
                    }
                }
                
                //______________________________________________________________
                //
                // fusion
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
            }
        }
        
    }
    
}


