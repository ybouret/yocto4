#include "yocto/lang/syntax/langen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/pattern/logic.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            LanGen:: ~LanGen() throw() {}


            LanGen:: LanGen(const xnode *node ) :
            root(node),
            P(0),
            rules(),
            rxp(),
            raw()
            {
                assert(root!=NULL);
                P.reset( new parser("dummy","main") );
                collect(root);
                find_rules_from(root);
                
                P->gramviz("langen.dot");
                (void) system("dot -Tpng -o langen.png langen.dot");
            }

            // in order collection
            void LanGen:: collect(const xnode *node)
            {
                if(node->terminal)
                {
                    //__________________________________________________________
                    //
                    // a non terminal rule
                    //__________________________________________________________
                    if(node->label=="ID")
                    {
                        const string r_id = node->lex().to_string();
                        if(!rules.search(r_id))
                        {
                            aggregate     *p = new aggregate(r_id);
                            const rule_ptr q( p );
                            if(!rules.insert(q))
                                throw exception("unexpected RULE '%s' insertion failure!", r_id.c_str());

                            // make the new rule
                            P->append(p);
                            p->withhold();
                        }
                        return;
                    }

                    //__________________________________________________________
                    //
                    // a terminal regexp
                    //__________________________________________________________
                    if(node->label=="RXP" )
                    {
                        const string t_id = node->lex().to_string();
                        if(!rxp.search(t_id))
                        {
                            terminal      *p = new terminal(t_id,standard);
                            const term_ptr q( p );
                            if(!rxp.insert(q))
                            {
                                if(!rxp.insert(q))
                                    throw exception("unexpected RegExp TERM '%s' insertion failure!", t_id.c_str());

                            }

                            // make the terminal from regexp
                            const char *id = p->label.c_str();
                            P->scanner.emit(id,id);
                            P->append( p );
                            p->withhold();
                        }
                        return;
                    }

                    //__________________________________________________________
                    //
                    // a terminal raw
                    //__________________________________________________________
                    if(node->label=="RAW" )
                    {
                        const string t_id = node->lex().to_string();
                        if(!raw.search(t_id))
                        {
                            terminal      *p = new terminal(t_id,standard);
                            const term_ptr q(p);
                            if(!raw.insert(q))
                            {
                                if(!raw.insert(q))
                                    throw exception("unexpected Raw TERM '%s' insertion failure!", t_id.c_str());
                            }

                            //make the terminal from raw
                            const lexical::action emit( &(P->scanner), &lexical::scanner::forward);
                            P->scanner.make(p->label, lang::logical::equal(p->label), emit);
                            P->append( p);
                            p->withhold();

                        }
                        return;
                    }

                    // a modifier....
                    std::cerr << "\tunregistered " << node->label << "(" << node->lex() << ")" << std::endl;

                }
                else
                {
                    for(const xnode *child = node->children().head; child;child=child->next)
                    {
                        collect(child);
                    }
                }

            }


        }
    }
}


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            void LanGen:: find_rules_from(const xnode *node)
            {
                if(node->terminal)
                    return;

                if("RULE"==node->label)
                {
                    build_rule_from(node);
                }
                else
                {
                    for(const xnode *child = node->children().head; child; child=child->next)
                    {
                        find_rules_from(child);
                    }
                }
            }


            void LanGen :: build_rule_from(const xnode *node)
            {
                //______________________________________________________________
                //
                // get rule ID
                //______________________________________________________________
                assert("RULE"==node->label);
                const xnode::leaves &children = node->children();
                assert(children.size>=2);

                const xnode *child = children.head;
                assert("ID"==child->label);

                const string RuleID = child->lex().to_string();
                std::cerr << "\tBuilding rule '" << RuleID << "'" << std::endl;
                rule_ptr *pp = rules.search(RuleID);
                if(!pp) throw exception("unexpected failure to get '%s'", RuleID.c_str());

                //______________________________________________________________
                //
                // grow from top level
                //______________________________________________________________
                logical *parent = & (**pp);
                grow_rule(parent,child->next);
            }

            void LanGen:: grow_rule( logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert(sub->label=="SUB");

                const xnode::leaves &children = sub->children(); assert(children.size>0);
                const xnode         *child = children.head;      assert(child!=NULL);

                std::cerr << "\t\tChild Label=" << child->label << std::endl;
                if("ALT"==child->label)
                {
                    std::cerr << "\t\tfound alternation" << std::endl;
                    return;
                }
                else
                {
                    
                }


            }


        }

    }

}


