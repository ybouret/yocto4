#include "yocto/lang/syntax/langen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include <cstdlib>
#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            LanGen:: ~LanGen() throw() {}



            static const char *collect_keywords[] =
            {
                "ID",    // 0
                "RXP",   // 1
                "RAW"    // 2
            };


            static const char *grow_rule_keywords[] =
            {
                "SUB",
                "ITEM"
            };

            LanGen:: LanGen(const xnode *node ) :
            root(node),
            P(0),
            rules(),
            rxp(),
            raw(),
            cmph( YOCTO_PERFECT_HASHER_FOR(collect_keywords)   ),
            rmph( YOCTO_PERFECT_HASHER_FOR(grow_rule_keywords) ),
            indx(0)
            {
                assert(root!=NULL);

                std::cerr << "Collect MPH #nodes=" << cmph.nodes << std::endl;
                std::cerr << "Growing MPH #nodes=" << rmph.nodes << std::endl;

                P.reset( new parser("dummy","main") );
                collect(root);
                find_rules_from(root);


                P->gramviz("langen.dot");
                (void) system("dot -Tpng -o langen.png langen.dot");
            }

            //__________________________________________________________________
            //
            //
            // in order collection
            //
            //__________________________________________________________________
            void LanGen:: collect(const xnode *node)
            {
                if(node->terminal)
                {

                    switch( cmph(node->label) )
                    {
                        case 0: {
                            assert("ID"==node->label);
                            //__________________________________________________
                            //
                            // a non terminal rule
                            //__________________________________________________
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
                        } break;

                        case 1 : {
                            assert("RXP"==node->label);
                            //__________________________________________________
                            //
                            // a terminal regexp
                            //__________________________________________________
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

                        } break;


                        case 2 : {
                            assert("RAW" == node->label );
                            //__________________________________________________
                            //
                            // a terminal raw
                            //__________________________________________________
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
                        } break;

                        default:
                            assert(-1==cmph(node->label));
                            std::cerr << "unregistered " << node->label << " content='" << node->lex() << "'" << std::endl;
                    }


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

            //__________________________________________________________________
            //
            //
            // Scan the tree for named rules
            //
            //__________________________________________________________________
            void LanGen:: find_rules_from(const xnode *node)
            {
                if(node->terminal)
                    return;

                if("RULE"==node->label)
                {
                    build_rule_from(node); // then return
                }
                else
                {
                    for(const xnode *child = node->children().head; child; child=child->next)
                    {
                        find_rules_from(child);
                    }
                }
            }


            //__________________________________________________________________
            //
            //
            // build one named rule
            //
            //__________________________________________________________________
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

                //______________________________________________________________
                //
                // grow from top level
                //______________________________________________________________
                logical *parent = & get_std(child);
                std::cerr << "\t\tBuilding Rule for " << parent->label << std::endl;
                grow_rule(parent,child->next);
            }


            //__________________________________________________________________
            //
            //
            // build one named rule
            //
            //__________________________________________________________________
            void LanGen:: grow_rule( logical *parent, const xnode *node)
            {
                assert(parent);
                assert(node);
                std::cerr << "\t\t\tgrow_rule '" << node->label << "'" << std::endl;

                if("SUB"==node->label)
                {
                    const xnode::leaves &subs = node->children();assert(subs.size>0);
                    const xnode         *sub  = subs.head;


                    if( "ALT" == sub->label)
                    {
                        std::cerr << "\t\t\t -- new alternation" << std::endl;
                        logical &child = P->alt();
                        for(;sub;sub=sub->next)
                        {
                            assert("ALT"==sub->label);
                            grow_rule(&child,sub);
                        }
                        parent->append(child);
                    }
                    else
                    {
                        std::cerr << "\t\t\t -- new aggregation" << std::endl;
                        for(;sub;sub=sub->next)
                        {
                            grow_rule(parent,sub);
                        }
                    }
                    return;
                }
                
                if("ALT"==node->label)
                {
                    const string agg_label =  vformat("@sub#%u",++indx);
                    logical &r = P->agg(agg_label);
                    parent->append(r);
                    for(const xnode *ch = node->children().head;ch;ch=ch->next)
                    {
                        grow_rule(&r,ch);
                    }
                    return;
                }
                
                
                {
                    std::cerr << "\t\t\t -- new " << node->label << std::endl;

                    if( "RXP" == node->label)
                    {
                        parent->append( get_rxp(node) );
                        return;
                    }

                    if( "RAW" == node->label )
                    {
                        parent->append( get_raw(node) );
                        return;
                    }

                    if( "ID" == node->label )
                    {
                        parent->append( get_std(node) );
                        return;
                    }
                    
                    if("ITEM" == node->label)
                    {
                        assert(2==node->children().size);
                        const string itm_label =  vformat("@sub#%u",++indx);
                        logical &r = P->agg(itm_label);
                        grow_rule(&r,node->children().head);
                        
                        //const string &kind = node->children().label;
                        
                        
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

            logical & LanGen:: get_std(const xnode *child)
            {
                assert(child);
                assert("ID"==child->label);
                const string id = child->content();
                rule_ptr    *pp = rules.search(id);

                if(!pp) throw exception("unexpected failure to get RULE '%s'", id.c_str());

                return **pp;
            }

            rule & LanGen:: get_rxp(const xnode *child)
            {
                assert(child);
                assert("RXP"==child->label);
                const string id = child->content();
                term_ptr    *pp = rxp.search(id);
                
                if(!pp) throw exception("unexpected failure to RegExp TERM '%s'", id.c_str());
                
                return **pp;
            }
            
            rule & LanGen:: get_raw(const xnode *child)
            {
                assert(child);
                assert("RAW"==child->label);
                const string id = child->content();
                term_ptr    *pp = raw.search(id);
                
                if(!pp) throw exception("unexpected failure to Raw TERM '%s'", id.c_str());
                
                return **pp;
            }
            
            
        }
    }
}



