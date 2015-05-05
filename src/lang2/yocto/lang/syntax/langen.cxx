#include "yocto/lang/syntax/langen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/syntax/aggregate.hpp"

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
                populate();
                P->gramviz("langen.dot");
                (void) system("dot -Tpng -o langen.png langen.dot");
            }

            void LanGen:: collect(const xnode *node)
            {
                if(node->terminal)
                {
                    // a non terminal rule
                    if(node->label=="ID")
                    {
                        const string r_id = node->lex().to_string();
                        if(!rules.search(r_id))
                        {
                            const agg_ptr q( new aggregate(r_id) );
                            if(!rules.insert(q))
                                throw exception("unexpected RULE '%s' insertion failure!", r_id.c_str());
                        }
                        return;
                    }

                    // a terminal regexp
                    if(node->label=="RXP" )
                    {
                        const string t_id = node->lex().to_string();
                        if(!rxp.search(t_id))
                        {
                            const term_ptr q( new terminal(t_id,standard) );
                            if(!rxp.insert(q))
                            {
                                if(!rxp.insert(q))
                                    throw exception("unexpected RegExp TERM '%s' insertion failure!", t_id.c_str());
                            }
                        }
                        return;
                    }

                    // a terminal raw
                    if(node->label=="RAW" )
                    {
                        const string t_id = node->lex().to_string();
                        if(!raw.search(t_id))
                        {
                            const term_ptr q( new terminal(t_id,standard) );
                            if(!raw.insert(q))
                            {
                                if(!raw.insert(q))
                                    throw exception("unexpected RegExp TERM '%s' insertion failure!", t_id.c_str());
                            }
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

#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            void LanGen:: populate()
            {
                //______________________________________________________________
                //
                // insert the rules
                //______________________________________________________________
                for( agg_set::iterator i=rules.begin();i!=rules.end();++i)
                {
                    rule &r = **i;
                    P->append( &r );
                    r.withhold();
                }


                //______________________________________________________________
                //
                // insert the terminals from rxp
                //______________________________________________________________
                for( term_set::iterator j=rxp.begin();j!=rxp.end();++j)
                {
                    terminal &t = **j;
                    const char *id = t.label.c_str();
                    P->scanner.emit(id,id);
                    P->append( &t );
                    t.withhold();
                }

                //______________________________________________________________
                //
                // insert the terminals from raw
                //______________________________________________________________

                for( term_set::iterator j=raw.begin();j!=raw.end();++j)
                {
                    terminal &t = **j;
                    const lexical::action emit( &(P->scanner), &lexical::scanner::forward);
                    P->scanner.make(t.label, lang::logical::equal(t.label), emit);
                    P->append( &t );
                    t.withhold();
                }


                //______________________________________________________________
                //
                // use the tree to feed the rules
                //______________________________________________________________
                find_rules_from(root);
            }

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


            void LanGen :: build_rule_from(const xnode *xnode)
            {
                //______________________________________________________________
                //
                // get rule ID
                //______________________________________________________________
                assert("RULE"==xnode->label);
                const syntax::xnode::leaves &children = xnode->children();
                assert(children.size>=2);

                const syntax::xnode *child = children.head;
                assert("ID"==child->label);

                const string RuleID = child->lex().to_string();
                std::cerr << "\tBuilding rule '" << RuleID << "'" << std::endl;
                agg_ptr *pp = rules.search(RuleID);
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
            }


        }

    }

}


