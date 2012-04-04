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
        
        sublexer & lexer:: declare( const string &sub_name )
        {
            sublex lx( new sublexer( sub_name, this ) );
            if( ! lexdb.insert( lx ) )
                throw exception("lexer.declare( multiple '%s' )",  &sub_name[0] );
            return *lx;
        } 
        
        sublexer & lexer:: declare( const char *sub_name )
        {
            const string __name(sub_name);
            return declare( __name );
        }
        
        sublexer & lexer:: operator[]( const string &sub_name )
        {
            sublex *ppLx = lexdb.search( sub_name );
            if( !ppLx ) throw exception("no lexer['%s']", &sub_name[0] );
            return **ppLx;
        }
        
        sublexer & lexer:: operator[]( const char *sub_name )
        {
            const string __name( sub_name );
            return (*this)[ __name ];
        }
        
        void lexer:: jump( const string &sub_name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( sub_name );
            if( !ppLx ) throw exception("no lexer.jump('%s')", &sub_name[0] );
            //std::cerr << "jump '" << active->name << "' -> '" << (*ppLx)->name << "'" << std::endl;
            active = &(**ppLx);
            
        }
        
        void lexer:: call( const string &sub_name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( sub_name );
            if( !ppLx ) 
                throw exception("no lexer.call('%s')", &sub_name[0] );
            call_stack.push_back( active );
            //std::cerr << "call '" << active->name << "' -> '" << (*ppLx)->name << "'" << std::endl;
            active = &(**ppLx);
        }
        
        void lexer:: back()
        {
            assert( NULL != active );
            if( call_stack.size() <= 0 )
                throw exception("no lexer.back(from '%s')", & (active->name[0]) );
            //std::cerr << "back from '" << active->name << "' to '" << call_stack.back()->name << "'" << std::endl;
            active = call_stack.back();
            call_stack.pop_back();
        }
        
        
        void lexer:: run( source &src )
        {
            assert(active!=NULL);
            while( active->process(src) )
            {
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
            const char     *pId = & (plg->name[0] );
            
            if( plg->is_attached() )
            {
                throw exception("lexer:: plugin '%s' is already attached", pId);
            }
            
            //------------------------------------------------------------------
            // make a plugin
            //------------------------------------------------------------------
            if( ! plugins.insert( Mod ) )
            {
                throw exception("lexer::load( multiple '%s' )",  pId);
            }
            
            //------------------------------------------------------------------
            // make a lexer
            //------------------------------------------------------------------
            try 
            {
                if( ! lexdb.insert( Lex ) )
                    throw exception("plugin/lexer conflict for '%s'",pId );
            } catch (...) 
            {
                (void) lexdb.remove( plg->name );
                throw;
            }
            
            //------------------------------------------------------------------
            // attach it to this
            //------------------------------------------------------------------
            plg->attach(this);
            
            
        }
        
        const lexical::plugin &lexer:: get_plugin( const string &plugin_name ) const
        {
            const lexical::module *pMod = plugins.search( plugin_name );
            if( ! pMod )
                throw exception("lexer::get_plugin(NO '%s')", & plugin_name[0] );
            return **pMod;
        }
        
        
    }
}
