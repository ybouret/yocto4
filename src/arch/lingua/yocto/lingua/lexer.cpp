#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // setup
        //
        ////////////////////////////////////////////////////////////////////////
        
        lexer:: ~lexer() throw() {}
        
#define Y_LEX_CTOR() \
name(id),           \
line(1),           \
scan(0),          \
cache(),         \
scanners(),     \
init(0),       \
history()
        
        lexer:: lexer( const string &id ) :
        Y_LEX_CTOR()
        {
        }
        
        lexer:: lexer( const char *id ) :
        Y_LEX_CTOR()
        {
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // scanners API
        //
        ////////////////////////////////////////////////////////////////////////
        lexical::scanner *lexer:: fetch( const string &id ) const throw()
        {
            const lexical::scanner::ptr *ppScanner = scanners.search(id);
            if( !ppScanner ) return 0;
            return (lexical::scanner *) &(**ppScanner);
        }
        
        
        
        lexical::scanner & lexer::declare( const string &id )
        {
            if( fetch(id) )
            {
                throw exception("lexer::declare(multipler '%s')", id.c_str());
            }
            
            lexical::scanner *s = new lexical::scanner(id,line);
            const scanner_ptr p(s);
            if( !scanners.insert(p) )
                throw exception("lexer::get(unexpected failure in insert '%s')", id.c_str() );
            if(!init) init = s;
            if(!scan) scan = s;
            s->link_to(*this);
            return *s;
        }
        
        lexical::scanner & lexer::declare( const char *id )
        {
            const string ID(id);
            return declare(ID);
        }
        
        const lexical::scanner & lexer:: sub( const string &id ) const
        {
            lexical::scanner *s = fetch(id);
            if( !s ) throw exception("lexer.get(no '%s)", id.c_str());
            return *s;
        }
        
        const lexical::scanner & lexer:: sub( const char *id ) const
        {
            const string ID(id);
            return sub(ID);
        }
        
        
        void lexer:: reset() throw()
        {
            line = 1;
            for( scannerDB::iterator i = scanners.begin(); i != scanners.end(); ++i)
            {
                (*i)->reset();
            }
            history.free();
            cache.kill();
            scan = init;
        }
        
        const lexical::scanner & lexer:: current() const
        {
            if( !scan ) throw exception("lexer[%s]: no current scanner", name.c_str());
            return *scan;
        }
        
        const lexical::scanner & lexer:: first() const
        {
            if( !init ) throw exception("lexer[%s]: no first scanner", name.c_str());
            return *init;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // run-time scanners
        //
        ////////////////////////////////////////////////////////////////////////
        void lexer:: jump( const string &id )
        {
            assert(scan);
            scanner_ptr target = fetch(id);
            if( !target )
                throw exception("lexer[%s].jump(no '%s')", name.c_str(), id.c_str());
            scan = target;
        }
        
        
        void lexer:: call( const string &id )
        {
            assert(scan);
            scanner_ptr target = fetch(id);
            if( !target )
                throw exception("lexer[%s].call(no '%s')", name.c_str(), id.c_str());
            history.push_back(scan);
            scan = target;
        }
        
        void lexer:: back()
        {
            assert(scan);
            if( history.size() <= 0 )
                throw exception("lexer[%s].back(invalid from '%s')", name.c_str(), scan->name.c_str());
            scan = history.back();
            history.pop_back();
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // lexeme I/O
        //
        ////////////////////////////////////////////////////////////////////////
        bool lexer:: is_active( source &src )
        {
            if( cache.size > 0 )
                return true;
            else
            {
                lexeme *lx = get(src);
                if(lx)
                {
                    cache.push_front(lx);
                    return true;
                }
                else
                    return false;
            }
        }
        
        const lexeme *lexer::peek() const throw()
        {
            assert(cache.size>0);
            return cache.head;
        }
        
        const lexeme *lexer:: last() const throw()
        {
            assert(cache.size>0);
            return cache.tail;;
        }
        
        void lexer:: unget( lexeme *lx ) throw()
        {
            assert(lx);
            cache.push_front(lx);
        }
        
        
        void lexer:: emit(const lexical::scanner &from, const string &data)
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
                token tmp( data );
                
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
        
        lexeme * lexer:: get( source &src )
        {
            
            while(true)
            {
                //--------------------------------------------------------------
                // check the scanner
                //--------------------------------------------------------------
                if( !scan )
                    throw exception("%u: lexer[%s] no scanner", unsigned(line), name.c_str());
                
                if( cache.size > 0 )
                    return cache.pop_front();
                
                bool    fctl = false;
                lexeme *lx   = scan->get(src, fctl);
                if( lx )
                {
                    assert(false==fctl);
                    return lx;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // no lexeme was detected
                    //
                    //----------------------------------------------------------
                    if( fctl )
                    {
                        //------------------------------------------------------
                        // that was a control pattern => continue
                        //------------------------------------------------------
                        continue;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // end of source !
                        //------------------------------------------------------
                        assert( ! src.is_active() );
                        return 0;
                    }
                }
            }
            
                       
        }
        
        
    }
}
