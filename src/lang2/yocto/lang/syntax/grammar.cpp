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
            
            void grammar:: set_root( rule &r ) throw()
            {
                assert( rules.owns(&r) );
                rules.move_to_front(&r);
            }


        }

    }

}


#include "yocto/exception.hpp"
#include "yocto/lang/syntax/terminal.hpp"

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

            rule & grammar:: term( const string &label )
            {
                check_label(label);
                rule *r = new terminal(label);
                rules.push_back(r);
                return *r;
            }

            rule & grammar:: term( const char *label )
            {
                const string Label(label);
                return term(Label);
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
            
        }

    }

}

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            xnode *grammar:: accept( lexer &lxr, source &src, ios::istream &fp)
            {
                const char *gname = name.c_str();
                if(rules.size<=0) throw exception("[[%s]]: no top level rule!",gname);

                xnode          *tree = 0;
                const rule     &root = *(rules.head);

                if( !root.accept(tree,lxr,src,fp) )
                {
                    assert(NULL==tree);
                    throw exception("[[%s]]: syntax error", gname);
                }

                return tree;

            }
            
        }
    }
}



