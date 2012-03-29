#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto 
{
    
    namespace regex
    {
        lexer:: ~lexer() throw()
        {
        }
        
#define YOCTO_RX_LEXER_INIT() \
lexdb(),\
active(  &declare( init_id ) ),\
call_stack(),\
init( active ),\
plugins()
        
        lexer:: lexer( const string &init_id) :
        YOCTO_RX_LEXER_INIT()
        {
        }
        
        
        lexer:: lexer( const char *init_id) :
        YOCTO_RX_LEXER_INIT()
        {
        }
        
        void lexer:: reset() throw()
        {
            for( sublex_set::iterator i = lexdb.begin(); i != lexdb.end(); ++i )
            {
                (*i)->reset();
            }
            active = init;
            call_stack.free();
        }
        
        sublexer & lexer:: main() throw()
        {
            assert( init != NULL );
            return *init;
        }
        
        sublexer & lexer:: declare( const string &name )
        {
            sublex lx( new sublexer( name, this ) );
            if( ! lexdb.insert( lx ) )
                throw exception("lexer.declare( multiple '%s' )", &name[0] );
            return *lx;
        } 
        
        sublexer & lexer:: declare( const char *name )
        {
            const string __name(name);
            return declare( __name );
        }
        
        sublexer & lexer:: operator[]( const string &name )
        {
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) throw exception("no lexer['%s']", &name[0] );
            return **ppLx;
        }
        
        sublexer & lexer:: operator[]( const char *name )
        {
            const string __name( name );
            return (*this)[ __name ];
        }
        
        void lexer:: jump( const string &name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) throw exception("no lexer.jump('%s')", &name[0] );
            std::cerr << "jump '" << active->name << "' -> '" << (*ppLx)->name << "'" << std::endl;
            active = &(**ppLx);
            
        }
        
        void lexer:: call( const string &name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) 
                throw exception("no lexer.call('%s')", &name[0] );
            call_stack.push_back( active );
            std::cerr << "call '" << active->name << "' -> '" << (*ppLx)->name << "'" << std::endl;
            active = &(**ppLx);
        }
        
        void lexer:: back()
        {
            assert( NULL != active );
            if( call_stack.size() <= 0 )
                throw exception("no lexer.back(from '%s')", & (active->name[0]) );
            std::cerr << "back from '" << active->name << "' to '" << call_stack.back()->name << "'" << std::endl;
            active = call_stack.back();
            call_stack.pop_back();
        }
        
        
        void lexer:: run( source &src )
        {
            assert(active!=NULL);
            //std::cerr << "lexer::run '" << active->name << "'" << std::endl;
            while( active->process(src) )
            {
                //std::cerr << "lexer::run '" << active->name << "'" << std::endl;
            }
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Plugin API for lexer
        //
        ////////////////////////////////////////////////////////////////////////
        
        
        void lexer:: load( lexical::plugin *plg )
        {
            assert( plg != NULL );
            lexical::module Mod(plg);
            sublex          Lex(plg);
            
            // make a plugin
            if( ! plugins.insert( Mod ) )
            {
                throw exception("lexer::load( multiple '%s' )", & (plg->name[0] ) );
            }
            
            // make a lexer
            try 
            {
                if( ! lexdb.insert( Lex ) )
                    throw exception("plugin/lexer conflict for '%s'", & (plg->name[0] ) );
            } catch (...) 
            {
                (void) lexdb.remove( plg->name );
                throw;
            }
            
            // go
            plg->attach(this);
            
            
        }
        
        const lexical::plugin &lexer:: get_plugin( const string &name ) const
        {
            const lexical::module *pMod = plugins.search( name );
            if( ! pMod )
                throw exception("lexer::get_plugin(NO '%s')", & name[0] );
            return **pMod;
        }
        
        
    }
}
