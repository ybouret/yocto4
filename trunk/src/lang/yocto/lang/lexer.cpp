#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        lexer:: ~lexer() throw()
        {
        }
        
#define Y_LEXER_CTOR() name(id), line(1), scan(0), scanners(2,as_capacity), root(0)
        
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
            ptr->link_to(*this);
            
            if( !scanners.insert(ptr) )
                throw exception("lexer {%s}: multiple scanner '%s'", name.c_str(), id.c_str());
            lexical::scanner &ref = *ptr;
            if(!root)
            {
                root = &ref;
                scan = root;
            }
            return ref;
        }

        lexical::scanner & lexer:: declare(const char   *id)
        {
            const string ID(id);
            return declare(ID);
        }

        
        void lexer:: set_root(const string &id)
        {
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan)
                throw exception("{%s}.set_root(no '%s')", name.c_str(), id.c_str());
            root = & (**ppScan);
            scan = root;
        }
        
        bool lexer:: has( const string &id) const throw()
        {
            return scanners.search(id) != 0;
        }

        void lexer:: initialize() throw()
        {
            line = 1;
            scan = root;
            for( scanDB::iterator i=scanners.begin();i!=scanners.end();++i)
            {
                (**i).reset();
            }
            history.free();
        }
        
        void lexer::jump(const string &id)
        {
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan) throw exception("{%s}.jump(NO '%s')", name.c_str(), id.c_str());
            scan = & (**ppScan);
        }
        
        void lexer:: call( const string &id )
        {
            assert(scan);
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan) throw exception("{%s}.call(NO '%s')", name.c_str(), id.c_str());
            history.push_back(scan);
            scan = & (**ppScan);
        }
        
        
        void lexer:: back()
        {
            assert(scan);
            if(history.size()<=0)
                throw exception("{%s}.back(IMPOSSIBLE)", name.c_str());
            scan = history.back();
            history.pop_back();
        }
        
        
    }
}
