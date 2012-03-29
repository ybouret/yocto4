#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/source.hpp"

#include <iostream>

namespace yocto
{
    namespace regex
    {
        
        
        bool sublexer:: process( source &src )
        {
            //==================================================================
            //
            // check lexer
            //
            //==================================================================
            std::cerr << "[processing '" << name << "']" << std::endl;
            
            
            if( rules_.size <= 0 )
                throw exception("sublexer['%s'] is EMPTY", &name[0] );
            
            //==================================================================
            //
            // test input
            //
            //==================================================================
            if( !src.is_active() )
            {
                //--------------------------------------------------------------
                // No more input
                //--------------------------------------------------------------
                return false;
            }
            else 
            {
                pattern         *best = NULL;
                lexical::rule   *todo = NULL;
                lexical::rule   *r    = rules_.head; assert( r != NULL );
                
                //--------------------------------------------------------------
                //
                // find an accepting rule
                //
                //--------------------------------------------------------------
                while( r != NULL )
                {
                    pattern *p = r->motif;
                    assert( 0 == p->size );
                    if( p->accept(src) )
                    {
                        best = p;
                        todo = r;
                        break;
                    }
                    r=r->next;
                }
                
                //--------------------------------------------------------------
                //
                // no matching rule
                //
                //--------------------------------------------------------------
                if( !best )
                {
                    assert( src.peek() );
                    const char C = src.peek()->data;
                    if( C >= 32 && C < 127 )
                        throw exception( "in sublexer['%s']: invalid char '%c'", &name[0], C );
                    else
                        throw exception("in sublexer['%s']: invalid char 0x%02x", &name[0], unsigned(C) );
                }
                
                //--------------------------------------------------------------
                //
                // find a better rule ?
                //
                //--------------------------------------------------------------
                
                //--------------------------------------------------------------
                //-- restore source for next probe
                //--------------------------------------------------------------
                src.uncpy( *best );
                r=r->next;
                while( r != NULL )
                {
                    pattern *p = r->motif;
                    if( p->accept(src) )
                    {
                        //------------------------------------------------------
                        // new candidate
                        //------------------------------------------------------
                        if( p->size > best->size )
                        {
                            // new winner !
                            best->clear();
                            best = p;
                            todo = r;
                            src.uncpy( *best );
                        }
                        else
                        {
                            // too late !
                            src.unget( *p );
                            p->clear();
                        }
                    }
                    r = r->next;
                }
                
                //--------------------------------------------------------------
                //
                // apply the rule to do
                //
                //--------------------------------------------------------------
                todo->apply();            //! the code
                src.skip( best->size );   //! update the soruce
                best->clear();            //! final clean up
                
            
                
                return true;
            }
            
        }
        
    }
    
}
