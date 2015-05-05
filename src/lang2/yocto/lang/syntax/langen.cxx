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
            terms()
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
                    if(node->label=="RX")
                    {
                        const string t_id = node->lex().to_string();
                        if(!terms.search(t_id))
                        {
                            const term_ptr q( new terminal(t_id) );
                            if(!terms.insert(q))
                            {
                                if(!terms.insert(q))
                                    throw exception("unexpected TERM '%s' insertion failure!", t_id.c_str());
                            }
                        }
                        return;
                    }

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
                // insert the terminals
                //______________________________________________________________
                for( term_set::iterator j=terms.begin();j!=terms.end();++j)
                {
                    terminal &t = **j;
                    const char *id = t.label.c_str();
                    P->scanner.emit(id,id);
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
                    std::cerr << "Found Rule " << node->children().head->lex() << std::endl;
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
                
            }
        }

    }

}


