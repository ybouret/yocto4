#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar::item
        //
        ////////////////////////////////////////////////////////////////////////
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
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar
        //
        ////////////////////////////////////////////////////////////////////////
        grammar:: ~grammar() throw()
        {
        }
        
        grammar:: grammar( const string &id ) :
        name(id),
        rules(),
        items()
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
        
        syntax::terminal & grammar:: term( const string &id )
        {
            syntax::terminal *r = new syntax::terminal(id);
            add( r );
            return *r;
        }
        
        syntax::aggregate & grammar:: agg( const string &id )
        {
            syntax::aggregate *r = new syntax::aggregate(id);
            add(r);
            return *r;
        }
        
        syntax::alternate & grammar:: alt( const string &id )
        {
            syntax::alternate *r = new syntax::alternate(id);
            add(r);
            return *r;
        }
        
        
        syntax::rule & grammar:: operator[]( const string &id ) 
        {
            item *it = items.search(id);
            if( !it ) throw exception("%s(no rule '%s')", name.c_str(), id.c_str() );
            assert(it->rule);
            return *(it->rule);
        }
        
        syntax::rule & grammar:: operator[]( const char *id ) 
        {
            const string ID(id);
            return (*this)[ ID ];
        }
        
        syntax::optional & grammar:: opt( const string &id, const string &src )
        {
            syntax::rule     &ref = (*this)[ src ];
            syntax::optional *jk  = new syntax::optional( id, ref );
            add(jk);
            return *jk;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar accept
        //
        ////////////////////////////////////////////////////////////////////////
        bool grammar:: accept( lexer &Lexer, regex::source &Source )
        {
            if( rules.size <= 0 )
                throw exception("empty grammar '%s'", name.c_str() );
            
            syntax::parse_node *Tree = NULL;
            if( rules.head->match(Lexer, Source, Tree) ) 
            {
                if( Tree )
                {
                    delete Tree;
                }
                return true;
            }
            else 
            {    
                assert( NULL == Tree );
                return false;
            }
        }
        
    }
    
}
