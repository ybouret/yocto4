#include "yocto/lingua/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar item
        //
        ////////////////////////////////////////////////////////////////////////
        grammar::item:: ~item() throw() {}
        grammar::item:: item( syntax::rule::ptr p ) throw() : handle(p) { assert(handle); }
        grammar::item:: item( const item &other ) throw() : handle( other.handle ) {}
        
        const string & grammar:: item:: key() const throw()
        {
            assert(handle);
            return handle->label;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar
        //
        ////////////////////////////////////////////////////////////////////////
        
        grammar:: ~grammar() throw()
        {
        }
        
#define Y_LINGUA_GRAMMAR_CTOR() \
name(id), \
rules(), \
items(), \
__eof("EOF"), \
opidx(0)
        
        grammar:: grammar( const string &id ) :
        Y_LINGUA_GRAMMAR_CTOR()
        {
        }
        
        grammar:: grammar( const char *id ) :
        Y_LINGUA_GRAMMAR_CTOR()
        {
        }
        
        void grammar:: add( syntax::rule *r )
        {
            assert(r);
            try
            {
                const item it(r);
                if( !items.insert(it) )
                    throw exception("grammar(multiple rule '%s')", r->label.c_str());
            }
            catch(...)
            {
                delete r;
                throw;
            }
            rules.push_back(r);
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // rules handling
        //
        ////////////////////////////////////////////////////////////////////////
        void grammar:: set_root( const string &rule_id )
        {
            item *it = items.search(rule_id);
            if(!it)
                throw exception("{%s}.set_root(no '%s')", name.c_str(), rule_id.c_str());
            assert(it->handle);
            rules.move_to_front(it->handle);
        }
        
        void grammar:: set_root( const char *rule_id )
        {
            const string ID(rule_id); set_root(ID);
        }
        
        void grammar:: set_root( const syntax::rule &r )
        {
            assert( rules.owns( &r) );
            rules.move_to_front( (syntax::rule *) &r );
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar terminal
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::terminal & grammar:: term( const string &id, syntax::node_property ppty)
        {
            syntax::terminal *r = new syntax::terminal(id,ppty,__eof);
            add(r);
            return *r;
        }
        
        syntax::terminal & grammar:: term( const char *id, syntax::node_property ppty)
        {
            const string ID(id);
            return term(ID,ppty);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar aggregate
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::aggregate & grammar:: agg(const string         &id,
                                          syntax::node_property ppty)
        {
            syntax::aggregate *r = new syntax::aggregate(id,ppty);
            add(r);
            return *r;
        }
        
        syntax::aggregate & grammar:: agg( const char *id, syntax::node_property ppty)
        {
            const string ID(id);
            return agg(ID,ppty);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar alternative
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::alternative & grammar:: alt( const string &id )
        {
            syntax::alternative *r = new syntax::alternative(id);
            add(r);
            return *r;
        }
        
        syntax::alternative & grammar:: alt( const char *id )
        {
            const string ID(id);
            return alt(ID);
        }
        
        syntax::alternative & grammar:: alt()
        {
            const string id = vformat("alt#%d", ++opidx);
            return alt(id);
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar optional
        //
        ////////////////////////////////////////////////////////////////////////
        void grammar:: check_ownership( const syntax::rule &p ) const
        {
            if( !rules.owns(&p) )
            {
                throw exception("{%s}: not owning rule '%s'", name.c_str(), p.label.c_str());
            }
        }
        
        syntax::optional &grammar:: opt( syntax::rule &p)
        {
            check_ownership(p);
            const string id = "opt@" + p.label;
            syntax::optional *r = new syntax::optional(id,p);
            add(r);
            return *r;
        }
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar repeating
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::repeating & grammar:: rep(  const string &id, syntax::rule &p, size_t at_least)
        {
            check_ownership(p);
            syntax::repeating *r = new syntax::repeating(id,p,at_least);
            add(r);
            return *r;
        }
        
        syntax::repeating & grammar:: rep(  const char *id, syntax::rule &p, size_t at_least)
        {
            const string ID(id);
            return rep(ID,p,at_least);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // access to named rules
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::rule & grammar:: operator[]( const string &id )
        {
            item *it = items.search(id);
            if(!it)
                throw exception("{%s} no rule '%s'", name.c_str(), id.c_str());
            return *(it->handle);
        }
        
        syntax::rule & grammar:: operator[]( const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar parsing
        //
        ////////////////////////////////////////////////////////////////////////
        
        syntax::xnode *grammar:: accept(lexer &Lexer, source &Source)
        {
            
            if( rules.size <= 0)
                throw exception("{%s}.parse(no rules)", name.c_str());
            
            syntax::xnode *Tree = 0;
            //==================================================================
            // try to accept root node
            //==================================================================
            if( rules.head->match(Lexer, Source, Tree) )
            {
                auto_ptr<syntax::xnode> root(Tree);
                if( Lexer.is_active(Source) && Lexer.peek()->label != __eof )
                {
                    const lexeme *lx = Lexer.peek();
                    const string  s  = lx->to_string();
                    throw exception("%u: {%s} illegal extraneous '%s'='%s'",
                                    unsigned(lx->line) ,
                                    name.c_str(),
                                    lx->label.c_str(),
                                    s.c_str() );
                }
                //std::cerr << "[[ SUCCESS ]]" << std::endl;
                return root.yield(); // may be null => empty
            }
            //==================================================================
            // something went wrong
            //==================================================================
            assert(0==Tree);
            std::cerr << std::endl << "---- NOT MATCHING" << std::endl;
            if(Lexer.is_active(Source))
            {
                std::cerr << "Lexemes: " << std::endl;
                for( const lexeme *lx = Lexer.peek(); lx; lx=lx->next)
                {
                    std::cerr << "\t'" << lx->label << "'=" << *lx << std::endl;
                }
                
                const lexeme *lx_last = Lexer.last(); assert(lx_last);
                const lexeme *lx_prev = lx_last->prev;
                if(lx_prev)
                {
                    throw exception("%u: {%s} invalid '%s' after '%s'",
                                    unsigned(lx_last->line),
                                    name.c_str(),
                                    lx_last->label.c_str(),
                                    lx_prev->label.c_str());
                }
                else
                {
                    throw exception("%u: {%s} invalid '%s'",
                                    unsigned(lx_last->line),
                                    name.c_str(),
                                    lx_last->label.c_str());
                }
            }
            else
            {
                throw exception("%u: {%s} no lexeme", unsigned(Lexer.line), name.c_str() );
            }
            
            
            
        }
        
        
    }
    
}
