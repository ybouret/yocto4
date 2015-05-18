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
                "RAW",   // 2
                "?",     // 3
                "*",     // 4
                "+"      // 5
            };


            static const char *grow_rule_keywords[] =
            {
                "SUB",  //0
                "ALT",  //1
                "RXP",  //2
                "RAW",  //3
                "ID",   //4
                "ITEM"  //5

            };

            static const char *joker_keywords[] =
            {
                "?",
                "*",
                "+"
            };


            LanGen:: LanGen(const xnode *node ) :
            root(node),
            P(0),
            rules(),
            rxp(),
            raw(),
            cmph( YOCTO_PERFECT_HASHER_FOR(collect_keywords)   ),
            rmph( YOCTO_PERFECT_HASHER_FOR(grow_rule_keywords) ),
            jmph( YOCTO_PERFECT_HASHER_FOR(joker_keywords)     ),
            name(0),
            indx(0),
            jndx(0),
            visited()
            {
                assert(root!=NULL);

                //std::cerr << "Collect MPH #nodes=" << cmph.nodes << std::endl;
                //std::cerr << "Growing MPH #nodes=" << rmph.nodes << std::endl;

                //______________________________________________________________
                //
                // Find the parser name and create it
                //______________________________________________________________
                assert(!root->terminal);
                assert("parser"==root->label);
                {
                    const xnode *ch = root->children().head;
                    assert(ch);
                    assert("NAME"==ch->label);
                    const string parser_name = ch->content(1,0);
                    const string parser_main = "main";
                    std::cerr << "\t### " << parser_name << " ###" << std::endl;
                    P.reset( new parser(parser_name,parser_main) );
                }

                //______________________________________________________________
                //
                // First Pass: raw build
                //______________________________________________________________
                name = P->grammar::name.c_str();
                collect(root);
                find_rules_from(root);

                //______________________________________________________________
                //
                //check existing rules
                //______________________________________________________________
                for( rule *r = & P->top_level(); r; r=r->next )
                {
                    check_valid_declared(r);
                }
                P->cleanup();

                P->gramviz("lanraw.dot");
                (void) system("dot -Tpng -o lanraw.png lanraw.dot");

                //______________________________________________________________
                //
                // Second Pass: remove useless sub rules
                //______________________________________________________________
                visited.ensure( P->count() );
                simplify(& P->top_level() );
                P->cleanup();



                //______________________________________________________________
                //
                // Third Pass: change semantic meanings (RAW...)
                //______________________________________________________________
                visited.free();
                for( rule *r = & P->top_level(); r; r=r->next )
                {
                    semantic(r);
                }
                P->gramviz("langen.dot");
                (void) system("dot -Tpng -o langen.png langen.dot");
            }


            void LanGen:: check_valid_declared(rule *r)
            {
                assert(r);
                if( !rules.search(r->label) )
                {
                    return;
                }

                switch(r->uuid)
                {
                    case aggregate::UUID:
                    case alternate::UUID:
                    {
                        const operands *ops = (operands *)(r->content());
                        if(ops->size<=0)
                        {
                            throw exception("%s: void rule '%s'", name, r->label.c_str());
                        }
                    } break;

                    default:
                        break;
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
                            property     ppty = standard;
                            const token &itkn = node->lex();
                            assert(itkn.size>0);
                            // checking property
                            if('%'==itkn.head->code)
                            {
                                ppty = mergeOne;
                                delete ((token&)itkn).pop_front();
                            }
                            const string r_id = node->content();
                            rule_ptr    *agg = rules.search(r_id);
                            if(!agg)
                            {
                                std::cerr << "+ID=" << r_id << ", ppty=" << int(ppty) << std::endl;

                                aggregate     *p = new aggregate(r_id,ppty);
                                P->append(p);
                                const rule_ptr q( p );
                                if(!rules.insert(q))
                                {
                                    throw exception("%s: unexpected RULE '%s' insertion failure!",name,r_id.c_str());
                                }
                            }
                            else
                            {
                                property &curr = (property &)(**agg).modifier;
                                if(curr==standard&&ppty!=standard)
                                {
                                    std::cerr << "@ID=" << r_id << " : change ppty" << std::endl;
                                    curr = ppty;
                                }
                            }
                        } break;

                        case 1 : {
                            assert("RXP"==node->label);
                            //__________________________________________________
                            //
                            // a terminal regexp
                            //__________________________________________________
                            const string t_id = node->content();
                            if(!rxp.search(t_id))
                            {
                                terminal      *p = new terminal(t_id,standard);
                                P->append(p);
                                const term_ptr q( p );
                                if(!rxp.insert(q))
                                {
                                    if(!rxp.insert(q))
                                    {
                                        throw exception("%s: unexpected RegExp TERM '%s' insertion failure!",name,t_id.c_str());
                                    }
                                }

                                // make the terminal from regexp
                                const char *id = p->label.c_str();
                                P->scanner.emit(id,id);

                            }

                        } break;


                        case 2 : {
                            assert("RAW" == node->label );
                            //__________________________________________________
                            //
                            // a terminal raw
                            //__________________________________________________
                            const string t_id = node->content();
                            if(!raw.search(t_id))
                            {
                                terminal      *p = new terminal(t_id,univocal);
                                P->append(p);
                                const term_ptr q(p);
                                if(!raw.insert(q))
                                {
                                    if(!raw.insert(q))
                                        throw exception("%s: unexpected Raw TERM '%s' insertion failure!",name,t_id.c_str());
                                }

                                //make the terminal from raw
                                const lexical::action emit( &(P->scanner), &lexical::scanner::forward);
                                P->scanner.make(p->label, lang::logical::equal(p->label), emit);
                            }
                        } break;

                        case 3:
                        case 4:
                        case 5:
                            break;

                        default:
                            assert(-1==cmph(node->label));
                            std::cerr << "unregistered " << node->label << " content='" << node->lex() << "'" << std::endl;
                    }


                }
                else
                {
                    if(node->label!="LXR")
                    {
                        for(const xnode *child = node->children().head; child;child=child->next)
                        {
                            collect(child);
                        }
                    }
                    else
                    {
                        compile_lexical(node);
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
            void LanGen:: compile_lexical(const xnode *node)
            {
                assert("LXR"==node->label);
                assert(2==node->children().size);

                const string code = node->children().head->content();
                std::cerr << "\tLEXICAL: " << code << std::endl;

                auto_ptr<pattern> p( compile_lexical_pattern(node->children().tail) );

                if(code=="@drop")
                {
                    const string code_id = code + vformat("/%u",++jndx);
                    const lexical::action drop( & P->scanner, & lexical::scanner::discard);
                    P->scanner.make(code_id, p.yield(), drop);
                    return;
                }

                if(code=="@endl")
                {
                    const string code_id = code + vformat("/%u",++jndx);
                    const lexical::action endl( & P->scanner, & lexical::scanner::newline);
                    P->scanner.make(code_id, p.yield(), endl);
                    return;
                }


                throw exception("%s: unregisterd code '%s'", name, code.c_str());
            }

            pattern *LanGen:: compile_lexical_pattern(const xnode *node)
            {
                if("RXP"==node->label)
                {
                    const string expr = node->content();
                    std::cerr << "\t\tcompiling \"" << expr << "\"" << std::endl;
                    return regexp(expr, & (P->dict) );
                }

                if("RAW"==node->label)
                {
                    const string expr = node->content();
                    std::cerr << "\t\tcompiling '" << expr << "'" << std::endl;
                    return lang::logical::equal(expr);
                }

                throw exception("%s: Unexpected Lexical Rule Failure",name);
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


                switch( rmph(node->label) )
                {
                    case 0: grow_sub(parent,node); break;

                    case 1: grow_alt(parent,node); break;

                    case 2: {
                        assert("RXP"==node->label);
                        //______________________________________________________
                        //
                        // a new RXP node
                        //______________________________________________________
                        parent->add( get_rxp(node) );
                    } break;

                    case 3: {
                        assert("RAW"==node->label);
                        //______________________________________________________
                        //
                        // a new RAW node
                        //______________________________________________________
                        parent->add( get_raw(node) );
                    } break;


                    case 4: {
                        assert("ID"==node->label);
                        //______________________________________________________
                        //
                        // a new ID node
                        //______________________________________________________
                        parent->add( get_std(node) );
                    } break;

                    case 5: grow_itm(parent,node); break;

                    default: assert(-1==rmph(node->label));
                        throw exception("%s: unexpected node '%s'", name, node->label.c_str());
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

                if(!pp) throw exception("%s: unexpected failure to get RULE '%s'", name, id.c_str());

                return **pp;
            }

            rule & LanGen:: get_rxp(const xnode *child)
            {
                assert(child);
                assert("RXP"==child->label);
                const string id = child->content();
                term_ptr    *pp = rxp.search(id);

                if(!pp) throw exception("%s: unexpected failure to RegExp TERM '%s'",name, id.c_str());

                return **pp;
            }

            rule & LanGen:: get_raw(const xnode *child)
            {
                assert(child);
                assert("RAW"==child->label);
                const string id = child->content();
                term_ptr    *pp = raw.search(id);

                if(!pp) throw exception("%s: unexpected failure to Raw TERM '%s'", name, id.c_str());

                return **pp;
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
            // a new SUB node
            //__________________________________________________________________
            void LanGen:: grow_sub(logical *parent, const xnode *node)
            {
                assert("SUB"==node->label);

                const xnode::leaves &subs = node->children();assert(subs.size>0);
                const xnode         *sub  = subs.head;


                if( "ALT" == sub->label)
                {
                    logical &child = P->alt();
                    for(;sub;sub=sub->next)
                    {
                        assert("ALT"==sub->label);
                        grow_rule(&child,sub);
                    }
                    parent->add(child);
                }
                else
                {
                    for(;sub;sub=sub->next)
                    {
                        grow_rule(parent,sub);
                    }
                }
            }

            //__________________________________________________________________
            //
            // a new ALT node
            //__________________________________________________________________
            void LanGen:: grow_alt( logical *parent, const xnode *node)
            {
                assert("ALT"==node->label);
                const string agg_label =  vformat("@sub#%u",++indx);
                logical &r = P->agg(agg_label,mergeOne);
                parent->add(r);

                for(const xnode *ch = node->children().head;ch;ch=ch->next)
                {
                    grow_rule(&r,ch);
                }
            }

            //__________________________________________________________________
            //
            // a new ITEM node
            //__________________________________________________________________
            void LanGen:: grow_itm( logical *parent, const xnode *node)
            {
                assert("ITEM"==node->label);
                assert(2==node->children().size);


                const string itm_label = vformat("@sub#%u",++indx);
                logical     &itm       = P->agg(itm_label,mergeOne);
                grow_rule(&itm,node->children().head);
                const string &kind = node->children().tail->label;

                // simplify at this point...
                rule *r = &itm;
                if(itm.size==1)
                {
                    logical::operand *op = itm.pop_front();
                    r = op->addr;
                    delete op;
                }
                assert(r);

                switch( jmph(kind) )
                {
                    case 0: assert("?"==kind); parent->add( P->opt(*r) );          break;
                    case 1: assert("*"==kind); parent->add( P->zero_or_more(*r) ); break;
                    case 2: assert("+"==kind); parent->add( P->one_or_more(*r)  ); break;
                    default:
                        assert(-1==jmph(kind));
                        throw exception("%s: invalid modifier '%s'", name, kind.c_str());
                }

            }



        }

    }

}

#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            static inline bool is_internal_sub( const string &label )
            {
                static const char   sub[] = "@sub#";
                static const size_t len   = sizeof(sub)/sizeof(sub[0])-1;
                if(label.size()>=len)
                {
                    for(size_t i=0;i<len;++i)
                    {
                        if( sub[i] != label[i] )
                            return false;
                    }
                    return true;
                }
                return false;
            }

            void LanGen:: simplify( rule *r )
            {
                assert(r);

                if(visited.search(r)) return;
                if(!visited.insert(r))
                {
                    throw exception("%s: unexpected simplify/visited failure for '%s'", name, r->label.c_str());
                }

                switch (r->uuid)
                {
                    case aggregate::UUID:
                    case alternate::UUID: {
                        //______________________________________________________
                        //
                        // simplify children
                        //______________________________________________________
                        operands *ops = (operands *)(r->content());
                        for( logical::operand *ch = ops->head; ch; ch=ch->next)
                        {
                            simplify(ch->addr);
                        }

                        //______________________________________________________
                        //
                        // check internal sub children with 1 child
                        //______________________________________________________
                        operands stk;
                        while(ops->size)
                        {
                            logical::operand *ch  = ops->pop_front();
                            rule             *sub = ch->addr;
                            if( is_internal_sub(sub->label) )
                            {
                                //std::cerr << "\t\tINTERNAL " << sub->label << std::endl;
                                operands *children = (operands *)(sub->content()); assert(children);
                                if(1==children->size)
                                {
                                    //std::cerr << "\t\tfusion!" << std::endl;
                                    stk.push_back(children->pop_front());
                                    delete ch;
                                }
                                else
                                {
                                    stk.push_back(ch);
                                }
                            }
                            else
                            {
                                stk.push_back(ch);
                            }
                        }

                        ops->swap_with(stk);


                    }   break;

                    case optional::UUID:
                    case at_least::UUID:
                        simplify( (rule*)(r->content()) );
                        break;


                    default:

                        break;
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
            void LanGen::set_jettison(rule *sub)
            {
                assert(sub);
                assert(raw.search(sub->label));
                assert(!visited.search(sub));

                property *modif = (property *)(sub->content());
                assert(modif!=NULL);
                assert(univocal==*modif || jettison==*modif);
                *modif = jettison;
                if(!visited.insert(sub))
                {
                    throw exception("%s: unexpected visited semantic failure for '%s'", name, sub->label.c_str());
                }



            }

            void LanGen:: semantic(rule *r)
            {
                assert(r);
                //! one raw in a multiple operands aggregate
                if(aggregate::UUID==r->uuid)
                {
                    std::cerr << "\t\t\tscanning semantic " << r->label << std::endl;
                    operands *ops = (operands *)(r->content());
                    if(ops->size>1)
                    {
                        for( logical::operand *ch = ops->head; ch; ch=ch->next)
                        {
                            rule *sub = ch->addr;
                            if(visited.search(sub))
                            {
                                continue;
                            }

                            if(raw.search(sub->label))
                            {
                                std::cerr << "\t\t\tsetting " << sub->label << " to JETTISON" << std::endl;
                                set_jettison(sub);
                            }
                        }
                    }
                }

                //! alias: a single raw in a declared aggregate
                if(aggregate::UUID==r->uuid&&rules.search(r->label))
                {
                    operands *ops = (operands *)(r->content());
                    if(1==ops->size)
                    {
                        rule *sub = ops->head->addr;

                        if(!visited.search(sub) && raw.search(sub->label))
                        {
                            std::cerr << "\t\t\taliasing " << sub->label <<  std::endl;
                            set_jettison(sub);
                        }
                    }
                }


            }
        }
    }
}



