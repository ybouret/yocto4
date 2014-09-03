#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        lexer:: ~lexer() throw()
        {
        }
        
#define Y_LEXER_CTOR() name(id), line(1), scanners(2,as_capacity), root(0)
        
        lexer:: lexer(const string &id) :
        Y_LEXER_CTOR()
        {
        }
        
        lexer:: lexer( const char *id ) :
        Y_LEXER_CTOR()
        {
        }
        
        lexical::scanner & lexer:: declare(const string &id)
        {
            lexical::scanner::pointer ptr( new lexical::scanner(id,line) );
            if( !scanners.insert(ptr) )
                throw exception("lexer {%s}: multiple scanner '%s'", name.c_str(), id.c_str());
            lexical::scanner &ref = *ptr;
            if(!root) root = &ref;
            return ref;
        }

        lexical::scanner & lexer:: declare(const char   *id)
        {
            const string ID(id);
            return declare(ID);
        }

        
    }
}
