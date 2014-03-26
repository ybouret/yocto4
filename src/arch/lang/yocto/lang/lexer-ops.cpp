#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        
        void lexer:: jump( const string &id )
        {
            assert( scan != NULL );
            scan = fetch(id);
            //std::cerr << "[jump lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        void lexer:: call( const string &id )
        {
            assert(scan != NULL);
            lexical::scanner *next_scan = fetch(id);
            history.push(scan);
            scan = next_scan;
            //std::cerr << "[call lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        void lexer:: back()
        {
            assert( scan != NULL );
            if( history.size() <= 0 ) throw exception("%s: can't go back!", scan->name.c_str() );
            scan = history.peek();
            history.pop();
            //std::cerr << "[back lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        lexeme * lexer:: next_lexeme( regex::source &src )
        {
            assert( scan != NULL );
            
            
            while(true)
            {
                //--------------------------------------------------------------
                //-- check cache, since a rule can put a lexeme in it
                //--------------------------------------------------------------
                if( cache.size > 0 )
                    return cache.pop_front();
                
                //--------------------------------------------------------------
                //-- call the current scanner
                //--------------------------------------------------------------
                bool    fctl = false;
                lexeme *lx   = scan->next_lexeme(src,fctl);
                
                //--------------------------------------------------------------
                //-- do we have something ?
                //--------------------------------------------------------------
                if(lx) 
                {
                    assert(false==fctl);
                    return lx;
                }
                
                //--------------------------------------------------------------
                //-- or do we have a control lexeme ?
                //--------------------------------------------------------------
                if( !fctl )
                {
                    //-- no: this is the end
                    break;
                }
                
                //--------------------------------------------------------------
                //-- since we met a control lexeme, try again
                //-- for current may have change !
                //--------------------------------------------------------------
            }
            return NULL;
        }
        
        void  lexer::unget( const lexical::scanner &from, const string &data )
        {
            //------------------------------------------------------------------
            //-- create an empty lexeme
            //------------------------------------------------------------------
            lexeme *lx = new lexeme( from.name, line );
            
            //------------------------------------------------------------------
            //-- store it
            //------------------------------------------------------------------
            cache.push_front(lx);
            try 
            {
                //--------------------------------------------------------------
                //-- create a corresponding token
                //--------------------------------------------------------------
                regex::token tmp( data );
                
                //--------------------------------------------------------------
                //-- steal it
                //--------------------------------------------------------------
                lx->swap_with(tmp);
            }
            catch(...)
            {
                delete cache.pop_front();
                throw;
            }
            
        }
        
        void lexer:: unget( lexeme *lx ) throw()
        {
            assert(lx);
            cache.push_front( lx );
        }
        
		bool lexer:: is_active( regex::source &src )
		{
			if( cache.size > 0 )
				return true;
			else
			{
				lexeme *lx = next_lexeme(src);
				if( lx )
				{
					cache.push_front(lx);
					return true;
				}
				else
					return false;
			}
		}
        
        const lexeme * lexer:: peek() const throw()
        {
            return cache.head;
        }
        
        const lexeme * lexer:: get( int pos ) const throw()
        {
            if( pos > 0 )
            {
                size_t num = pos;
                if( num > cache.size )
                    return NULL;
                const lexeme *lx = cache.head;
                for( --num; num>0; --num )
                    lx=lx->next;
                return lx;
            }
            else
            {
                if( pos < 0 )
                {
                    size_t num = -pos;
                    if( num > cache.size )
                        return NULL;
                    const lexeme *lx = cache.tail;
                    for( --num; num>0; --num )
                        lx = lx->prev;
                    return lx;
                }
                else 
                {
                    assert(0==pos);
                    return NULL;
                }
            }
        }
    }
    
}
