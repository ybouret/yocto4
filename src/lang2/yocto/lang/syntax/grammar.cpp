#include "yocto/lang/syntax/grammar.hpp"

namespace yocto
{
    namespace lang
    {

        namespace syntax
        {


            grammar:: ~grammar() throw()
            {
            }


            grammar:: grammar(const string &id) :
            name(id),
            rules()
            {

            }

            grammar:: grammar(const char  *id) :
            name(id),
            rules()
            {

            }
            
            void grammar:: top_level( rule &r ) throw()
            {
                assert( rules.owns(&r) );
                rules.move_to_front(&r);
            }


        }

    }

}


#include "yocto/exception.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"

namespace yocto
{
    namespace lang
    {

        namespace syntax
        {
            void grammar:: check_label(const string &label) const
            {
                for(const rule *r=rules.head;r;r=r->next)
                {
                    if(label==r->label)
                    {
                        throw exception("grammar %s: multiple rule '%s'", name.c_str(), label.c_str());
                    }
                }
            }

            rule & grammar:: decl_term( const string &label )
            {
                check_label(label);
                rule *r = new terminal(label);
                rules.push_back(r);
                return *r;
            }

            rule & grammar:: decl_term( const char *label )
            {
                const string Label(label);
                return decl_term(Label);
            }

            aggregate & grammar:: agg( const string &label)
            {
                check_label(label);
                aggregate *r = new aggregate(label);
                rules.push_back(r);
                return *r;
            }

            aggregate & grammar:: agg( const char *label)
            {
                const string Label(label); return agg(Label);
            }


            alternate & grammar:: alt()
            {
                alternate *r = new alternate();
                rules.push_back(r);
                return *r;
            }

            rule & grammar:: opt(rule &r)
            {
                if( ! rules.owns(&r) )
                    throw exception("rule '%s' doesn't belong to grammar [[%s]]", r.label.c_str(), name.c_str());
                optional *R = new optional(r);
                rules.push_back(R);
                return *R;
            }

            rule & grammar:: at_least(rule &r, const size_t nmin)
            {
                if( ! rules.owns(&r) )
                    throw exception("rule '%s' doesn't belong to grammar [[%s]]", r.label.c_str(), name.c_str());
                rule *R = new syntax::at_least(r,nmin);
                rules.push_back(R);
                return *R;
            }

            rule & grammar:: zero_or_more(rule &r)
            {
                return at_least(r, 0);
            }

            rule & grammar:: one_or_more(rule &r)
            {
                return at_least(r, 1);
            }


        }

    }

}

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            xnode *grammar:: accept( lexer &lxr, source &src, ios::istream &fp)
            {
                //______________________________________________________________
                //
                // Let's go, try to accept the top level rules
                //______________________________________________________________
                const char *gname = name.c_str();
                if(rules.size<=0) throw exception("[[%s]]: no top level rule!",gname);

                xnode          *tree = 0;
                const rule     &root = *(rules.head);

                if( !root.accept(tree,lxr,src,fp) )
                {
                    assert(NULL==tree);
                    //__________________________________________________________
                    //
                    // not accepted: what happened ?
                    //__________________________________________________________
                    switch(lxr.size)
                    {
                        case 0:
                            throw exception("[[%s]]: no lexeme",gname);

                        case 1:
                            throw exception("[[%s]]: unexpected single lexeme %s", gname, lxr.head->label.c_str());

                        default:
                            assert(lxr.tail);
                            assert(lxr.tail->prev);
                            throw exception("[[%s]]: unexpected %s after %s", gname, lxr.tail->label.c_str(), lxr.tail->prev->label.c_str());

                    }

                    throw exception("[[%s]]: syntax error", gname);
                }

                //______________________________________________________________
                //
                // Let's study the result
                //______________________________________________________________
                auto_ptr<xnode> ast(tree);
                const lexeme *lx = lxr.peek(src, fp);
                if(lx)
                {
                    throw exception("%d:[[%s]]: unexpected extraneous %s", lx->line, gname, lx->label.c_str());
                }



                return ast.yield();

            }
            
        }
    }
}



