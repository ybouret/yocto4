#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        lexer:: ~lexer() throw()
        {
        }
        
        
#define Y_LEXER_CTOR() \
line(1), \
scanners(4,as_capacity), \
scan(NULL), \
cache(),\
history()
        
        
#define Y_LEXER_INIT() \
(void)  declare( main_id ); \
scan = & first()
        
        lexer:: lexer( const string &main_id ) :
        Y_LEXER_CTOR()
        {
            Y_LEXER_INIT();
        }
        
        lexer:: lexer( const char *main_id ) :
        Y_LEXER_CTOR()
        {
            Y_LEXER_INIT();
        }
        
        lexical::scanner & lexer:: first() throw()
        {
            return *scanners.front();
        }
        
        lexical::scanner & lexer:: declare( const string &id )
        {
            lexical::scanner::ptr p( new lexical::scanner( id, line ) );
            if( ! scanners.insert(p) )
                throw exception("lexer(multiple scanner '%s')", id.c_str() );
            p->link_to( *this );
            return *p;
        }
        
        lexical::scanner & lexer:: declare( const char *id )
        {
            const string ID(id);
            return declare(ID);
        }
        
        lexical::scanner * lexer:: fetch( const string &id ) const
        {
            const lexical::scanner::ptr *p = scanners.search(id);
            if( !p )
                throw exception("lexer(no scanner '%s')", id.c_str() );
            const lexical::scanner &s = **p;
            return (lexical::scanner *) &s;
        }
        
        lexical::scanner * lexer:: fetch( const char *id ) const
        {
            const string ID(id);
            return fetch(ID);
        }
        
        void lexer:: reset() throw()
        {
            line    = 1;
            scan    = & first();
            for( scannerDB::iterator i = scanners.begin(); i != scanners.end(); ++i )
            {
                (*i)->reset();
            }
            history.free();
        }
        
        const lexical::scanner & lexer:: current() const throw()
        {
            assert(NULL!=scan);
            return *scan;
        }
    }
    
}
