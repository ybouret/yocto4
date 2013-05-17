#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        lexer:: ~lexer() throw() {}
        
#define Y_LEX_CTOR() \
name(id),           \
line(1),           \
scan(0),          \
cache(),         \
scanners(),     \
init(0)
        
        lexer:: lexer( const string &id ) :
        Y_LEX_CTOR()
        {
        }
        
        lexer:: lexer( const char *id ) :
        Y_LEX_CTOR()
        {
        }
        
        
        lexical::scanner *lexer:: fetch( const string &id ) throw()
        {
            lexical::scanner::ptr *ppScanner = scanners.search(id);
            if( !ppScanner ) return 0;
            return &(**ppScanner);
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
        
        void lexer:: unget( lexeme *lx ) throw()
        {
            assert(lx);
            cache.push_front(lx);
        }
        
        lexeme * lexer:: get( source &src )
        {
            
            while(true)
            {
                if( !scan )
                    throw exception("%u: lexer[%s] no scanner", unsigned(line), name.c_str());
                std::cerr << "[" << name << "]<" << scan->name << ">" << std::endl;
                
                if( cache.size > 0 )
                    return cache.pop_front();
                
                bool    fctl = false;
                lexeme *lx  = scan->get(src, fctl);
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
