#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        grammar:: item:: ~item() throw()
        {
        }
        
        grammar:: item:: item( syntax::rule *r ) throw() :
        rule(r)
        {
            assert(r!=NULL || die("NULL rule"));
        }
        
        grammar:: item:: item( const item &it ) throw() :
        rule( it.rule )
        {
        }
        
        const string & grammar:: item:: key() const throw()
        {
            assert(rule!=NULL);
            return rule->label;
        }
        
        grammar:: ~grammar() throw()
        {
        }
        
        grammar:: grammar( const string &id ) :
        name(id),
        rules()
        {
        }
        
        void grammar:: add( syntax::rule *r )
        {
            assert(r!=NULL);
            try 
            {
                const item it(r);
                if( ! items.insert(it) )
                    throw exception("%s(Multiple rule '%s')", name.c_str(), r->label.c_str() );
            }
            catch(...){ delete r; throw; }
            rules.push_back(r);
        }
        
    }
    
}