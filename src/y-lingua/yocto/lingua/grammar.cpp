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
        
        grammar:: grammar( const string &id ) :
        name(id),
        rules(),
        items(),
        __eof("EOF")
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
        
        void grammar:: set_root( const syntax::rule &r )
        {
            assert( rules.owns( &r) );
            rules.move_to_front( (syntax::rule *) &r );
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar parsing
        //
        ////////////////////////////////////////////////////////////////////////
        
        syntax::xnode * grammar:: parse(lexer &Lexer, source &Source)
        {
            if( rules.size <= 0)
                throw exception("{%s}.parse(no rules)", name.c_str());
            
            syntax::xnode *Tree = 0;
            if( rules.head->match(Lexer, Source, Tree) )
            {
                auto_ptr<syntax::xnode> ans( Tree );
                if( Lexer.is_active(Source) && Lexer.peek()->label != __eof )
                {
                    const lexeme *lx = Lexer.peek();
                    throw exception("%u: {%s} illegal extraneous '%s'", unsigned(lx->line) , name.c_str(), lx->label.c_str() );
                }
                std::cerr << "[[ SUCCESS ]]" << std::endl;
                return ans.yield();                
            }
            
            throw exception("not matching");
        }
        
        
        
    }
    
}
