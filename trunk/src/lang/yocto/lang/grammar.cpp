#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        grammar:: ~grammar() throw()
        {
            
        }
        
        grammar:: grammar(const string &id) :
        name(id),
        rules()
        {
        }
        
        
        syntax::rule * grammar:: find_rule(const string &label ) const throw()
        {
            for( const syntax::rule *r=rules.head;r;r=r->next)
            {
                if(label==r->label)
                {
                    return (syntax::rule *)r;
                }
            }
            return 0;
        }
        
        void grammar:: set_root( const string &label )
        {
            syntax::rule *r = find_rule(label);
            if(!r)
            {
                throw exception("((%s)).set_root(no '%s')", name.c_str(), label.c_str());
            }
            rules.move_to_front(r);
        }
        
        
        void grammar:: set_root( const char *label )
        {
            const string Label(label);
            set_root(Label);
        }
        
        syntax::rule & grammar:: operator[](const string &label)
        {
            syntax::rule *r = find_rule(label);
            if(!r)
            {
                throw exception("no ((%s))['%s']",name.c_str(), label.c_str());
            }
            return *r;
        }
        
        syntax::rule & grammar:: operator[](const char *label)
        {
            const string Label(label);
            return (*this)[Label];
        }
        
        
        
    }
}
