#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace regex
    {
        namespace lexical
        {
            
            jump:: ~jump() throw()
            {
            }
            
            jump:: jump( pattern *p, const action &a, lexer **ppLx, const string &name ) :
            rule(p),
            on_enter(a),
            pp_lexer( ppLx ),
            sub_name( name )
            {
                assert( pp_lexer != NULL );
            }
            
            rule * jump:: create( pattern *p, const action &a, lexer **ppLx, const string &name ) 
            {
                assert(ppLx!=NULL);
                try 
                {
                    return new jump(p,a,ppLx,name);
                } 
                catch (...) 
                {
                    delete p;
                    throw;
                }
            }
            
            jump:: jump( const jump &other ) :
            rule( other ),
            on_enter( other.on_enter ),
            pp_lexer( other.pp_lexer ),
            sub_name( other.sub_name )
            {
                assert( pp_lexer != NULL );
            }
            
            rule * jump:: clone() const 
            {
                return new jump( *this );
            }

            
            void jump:: apply() 
            {
                assert( pp_lexer != NULL );
                lexer *lx = *pp_lexer;
                if( !lx ) 
                    throw exception("no lexer to jump('%s')", & sub_name[0] );
                lx->jump( sub_name );
                on_enter( *motif );
            }
            
            
            ////////////////////////////////////////////////////////////////////
            
            call:: ~call() throw()
            {
            }
            
            call:: call( pattern *p, const action &a, lexer **ppLx, const string &name ) :
            rule(p),
            on_enter(a),
            pp_lexer( ppLx ),
            sub_name( name )
            {
                assert( pp_lexer != NULL );
            }
            
            rule * call:: create( pattern *p, const action &a, lexer **ppLx, const string &name ) 
            {
                assert(ppLx!=NULL);
                try 
                {
                    return new call(p,a,ppLx,name);
                } 
                catch (...) 
                {
                    delete p;
                    throw;
                }
            }
            
            call:: call( const call &other ) :
            rule( other ),
            on_enter( other.on_enter ),
            pp_lexer( other.pp_lexer ),
            sub_name( other.sub_name )
            {
                assert( pp_lexer != NULL );
            }
            
            
            rule * call:: clone() const 
            {
                return new call( *this );
            }
            
            void call:: apply() 
            {
                assert( pp_lexer != NULL );
                lexer *lx = *pp_lexer;
                if( !lx ) 
                    throw exception("no lexer to call('%s')", & sub_name[0] );
                lx->call( sub_name );
                on_enter( *motif );
            }
            
            ////////////////////////////////////////////////////////////////////
            back:: ~back() throw()
            {
            }
            
            back:: back( pattern *p, const action &a, lexer **ppLx ) :
            rule(p),
            on_leave(a),
            pp_lexer( ppLx )
            {
                assert( pp_lexer != NULL );
            }
            
            rule * back:: create( pattern *p, const action &a, lexer **ppLx ) 
            {
                assert(ppLx!=NULL);
                try 
                {
                    return new back(p,a,ppLx);
                } 
                catch (...) 
                {
                    delete p;
                    throw;
                }
            }
            
            back:: back( const back &other ) :
            rule( other ),
            on_leave( other.on_leave ),
            pp_lexer( other.pp_lexer )
            {
                assert( pp_lexer != NULL );
            }
            
            rule * back:: clone() const
            {
                return new back( *this );
            }
            
            void back:: apply() 
            {
                assert( pp_lexer != NULL );
                lexer *lx = *pp_lexer;
                if( !lx ) 
                    throw exception("no lexer to go back");
                lx->back();
                on_leave( *motif );
            }
            
            
        }
    }
    
}

