#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        grammar:: ~grammar() throw()
        {
            
        }
        
#define Y_LANG_GRAMMAR_CTOR() \
name(id),\
rules(), \
optIndex(0)
        
        
        grammar:: grammar(const string &id) :
        Y_LANG_GRAMMAR_CTOR()
        {
        }
        
        void grammar:: show_rules() const
        {
            std::cerr << "((" << name << ")):" << std::endl;
            std::cerr << "{" << std::endl;
            for(const syntax::rule *r=rules.head;r;r=r->next)
            {
                std::cerr << "\t" << r->label << std::endl;
            }
            std::cerr << "}" << std::endl;
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
        
        void grammar:: set_root( const syntax::rule &root )
        {
            for(syntax::rule *r = rules.head; r; r=r->next)
            {
                if( r == &root )
                {
                    rules.move_to_front(r);
                    return;
                }
            }
            
            throw exception("((%s)).set_root(not found)", name.c_str());
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
        
        
        void grammar:: ensure_no(const string &label) const
        {
            if(find_rule(label))
                throw exception("((%s)) multiple rule '%s", name.c_str(), label.c_str() );
        }
        
        //----------------------------------------------------------------------
        syntax::terminal & grammar:: term( const string &label )
        {
            ensure_no(label);
            syntax::terminal *r = new syntax::terminal(label);
            rules.push_back( r );
            return *r;
        }
        
        syntax::terminal & grammar:: term( const char *label )
        {
            const string Label(label);
            return term(Label);
        }
        
        //----------------------------------------------------------------------
        syntax::optional & grammar:: opt( syntax::rule &other )
        {
            const string      label = name + ":" + other.label + vformat("?/#%d",++optIndex);
            ensure_no(label);
            syntax::optional *r     = new syntax::optional(label,other);
            rules.push_back(r);
            return *r;
        }
        
        //----------------------------------------------------------------------
        syntax::at_least & grammar:: at_least( const string &label, syntax::rule &other, size_t count)
        {
            ensure_no(label);
            syntax::at_least *r = new syntax::at_least(label,other,count);
            rules.push_back(r);
            return *r;
        }
        
        syntax::at_least & grammar:: at_least( const char *label, syntax::rule &other, size_t count)
        {
            const string Label(label);
            return at_least(Label,other,count);
        }
        
        syntax::at_least & grammar:: zero_or_more( const char *label, syntax::rule &other )
        {
            return at_least(label,other,0);
        }
        
        syntax::at_least & grammar:: one_or_more( const char *label, syntax::rule &other )
        {
            return at_least(label,other,1);
        }
        
		//----------------------------------------------------------------------
        syntax::aggregate & grammar:: agg(const string &label)
		{
			ensure_no(label);
			syntax::aggregate *r = new syntax::aggregate(label);
			rules.push_back(r);
			return *r;
		}

    }
}
