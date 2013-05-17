#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        lexer:: ~lexer() throw() {}
        
#define Y_LEX_CTOR() \
name(id), \
line(0), \
scan(0), \
scanners(), \
init(0)
        
        lexer:: lexer( const string &id ) :
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
            scan = init;
        }
        
        
        void lexer:: jump( const string &id )
        {
            assert(scan);
            scanner_ptr target = fetch(id);
            if( !target )
                throw exception("lexer::jump(no '%s')", id.c_str());
            scan = target;
        }
        
        
        void lexer:: call( const string &id )
        {
            assert(scan);
            scanner_ptr target = fetch(id);
            if( !target )
                throw exception("lexer::call(no '%s')", id.c_str());
            history.push_back(scan);
            scan = target;
        }
        
        void lexer:: back()
        {
            assert(scan);
            if( history.size() <= 0 )
                throw exception("lexer::back(invalid from '%s'", scan->name.c_str());
            scan = history.back();
            history.pop_back();
        }
        
        
    }
}
