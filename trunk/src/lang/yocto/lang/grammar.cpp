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
        
        
#define Y_GRAMMAR_CTOR() \
name(id),                \
rules(),                 \
items(),                 \
__eof("EOF")
        
        grammar:: grammar( const string &id ) :
        Y_GRAMMAR_CTOR() 
        {
        }
        
        grammar:: grammar( const char * id ) :
        Y_GRAMMAR_CTOR()
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
        
        //======================================================================
        // term
        //======================================================================
        syntax::terminal & grammar:: term( const string &id, syntax::node_property ppty )
        {
            syntax::terminal *r = new syntax::terminal(id,ppty,__eof);
            add( r );
            return *r;
        }
        
        syntax::terminal & grammar:: term( const char *id, syntax::node_property ppty )
        {
            const string ID(id);
            return term( ID, ppty );
        }
        
        //======================================================================
        // agg
        //======================================================================
        syntax::aggregate & grammar:: agg( const string &id, syntax::node_property ppty )
        {
            syntax::aggregate *r = new syntax::aggregate(id,ppty);
            add(r);
            return *r;
        }
        
        syntax::aggregate & grammar:: agg( const char *id, syntax::node_property ppty )
        {
            const string ID(id);
            return agg(ID,ppty);
        }
        
        
        //======================================================================
        // alt
        //======================================================================
        syntax::alternate & grammar:: alt( const string &id )
        {
            syntax::alternate *r = new syntax::alternate(id);
            add(r);
            return *r;
        }
        
        syntax::alternate & grammar:: alt( const char *id )
        {
            const string ID(id);
            return alt(ID);
        }
        
        //======================================================================
        // access
        //======================================================================
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
        
        //======================================================================
        // opt
        //======================================================================
        syntax::optional & grammar:: opt( const string &id, const string &src )
        {
            syntax::rule     &ref = (*this)[ src ];
            syntax::optional *jk  = new syntax::optional( id, ref );
            add(jk);
            return *jk;
        }
        
        syntax::optional & grammar:: opt( const char *id, const char *src )
        {
            const string ID(id);
            const string SRC(src);
            return opt(ID,SRC);
        }

        
        syntax::optional &grammar:: opt( const string &id, syntax::rule &ref )
        {
            if( ! rules.owns( &ref ) )
                throw exception("optional<%s>: unregistered rule <%s>", id.c_str(), ref.label.c_str() );
            syntax::optional *jk  = new syntax::optional( id, ref );
            add(jk);
            return *jk;
        }
        
        syntax::optional & grammar:: opt( const char *id, syntax::rule &ref )
        {
            const string ID(id);
            return opt(ID,ref);
        }
        
        //======================================================================
        // rep
        //======================================================================
        syntax::repeating  &grammar:: rep( const string &id, const string &src, size_t at_least)
        {
            syntax::rule      &ref = (*this)[ src ];
            syntax::repeating *jk  = new syntax::repeating( id, ref, at_least );
            add(jk);
            return *jk;
        }
        
        syntax::repeating  &grammar:: rep( const char *id, const char *src, size_t at_least )
        {
            const string ID(id);
            const string SRC(src);
            return rep(ID,SRC,at_least);
        }
        
        syntax::repeating &grammar:: rep( const string &id, syntax::rule &ref, size_t at_least)
        {
            if( ! rules.owns( &ref ) )
                throw exception("%s.rep<%s>: unregistered rule <%s>", name.c_str(), id.c_str(), ref.label.c_str() );
            syntax::repeating *jk  = new syntax::repeating( id, ref, at_least );
            add(jk);
            return *jk;
        }
        
        syntax::repeating  &grammar:: rep( const char *id, syntax::rule &ref, size_t at_least)
        {
            const string ID(id);
            return rep(ID,ref,at_least);
        }
        
        //======================================================================
        // set_root
        //======================================================================
        void grammar:: set_root( const string &id )
        {
            item *it = items.search(id);
            if( !it )
                throw exception("%s.set_root( NO '%s' )", name.c_str(), id.c_str() );
            rules.move_to_front( it->rule );
        }
        
        void grammar:: set_root( const char   *id )
        {
            const string ID(id);
            set_root(ID);
        }
        
        void grammar:: set_root( syntax::rule &r  )
        {
            if( ! rules.owns( &r ) )
                throw exception("%s.set_root(unregistered rule <%s>)", name.c_str(), r.label.c_str() );
            rules.move_to_front( &r );
        }
        
    }
    
}
