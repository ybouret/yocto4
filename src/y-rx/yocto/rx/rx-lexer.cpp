#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"

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
init( active )
        
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
        
        sub_lexer & lexer:: main() throw()
        {
            assert( init != NULL );
            return *init;
        }
        
        sub_lexer & lexer:: declare( const string &name )
        {
            sublex lx( new sub_lexer( name, this ) );
            if( ! lexdb.insert( lx ) )
                throw exception("lexer.declare( multiple '%s' )", &name[0] );
            return *lx;
        } 
        
        sub_lexer & lexer:: declare( const char *name )
        {
            const string __name(name);
            return declare( __name );
        }
        
        sub_lexer & lexer:: operator[]( const string &name )
        {
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) throw exception("no lexer['%s']", &name[0] );
            return **ppLx;
        }
        
        sub_lexer & lexer:: operator[]( const char *name )
        {
            const string __name( name );
            return (*this)[ __name ];
        }
        
        void lexer:: jump( const string &name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) throw exception("no lexer.jump('%s')", &name[0] );
            active = &(**ppLx);
        }
        
        void lexer:: call( const string &name )
        {
            assert( NULL != active );
            sublex *ppLx = lexdb.search( name );
            if( !ppLx ) 
                throw exception("no lexer.call('%s')", &name[0] );
            call_stack.push_back( active );
            active = &(**ppLx);
        }

        void lexer:: back()
        {
            assert( NULL != active );
            if( call_stack.size() <= 0 )
                throw exception("no lexer.back(from '%s')", & (active->name[0]) );
            active = call_stack.back();
            call_stack.pop_back();
        }
        
        
        void lexer:: run( source &src )
        {
            assert(active!=NULL);
            while( active->process(src) );
        }
        
        
    }
}
