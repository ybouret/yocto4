#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        bool  lexer:: emit(const token &) throw() { return true;  }
        bool  lexer:: drop(const token &) throw() { return false; }
        void  lexer:: newline() throw() { ++line; }
        bool  lexer:: newline_drop(const token&) throw() { newline(); return false; }
        bool  lexer:: newline_emit(const token&) throw() { newline(); return true;  }
        
        
        const lexical::scanner & lexer:: operator[](const string &id) const
        {
            const lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan)
            {
                throw exception("no {%s}.<%s>", name.c_str(), id.c_str());
            }
            return **ppScan;
        }
        
        const lexical::scanner & lexer:: operator[](const char *id) const
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        
        lexer:: ~lexer() throw()
        {
        }
        
#define Y_LEXER_CTOR() \
name(id), line(1), scan(0), scanners(2,as_capacity), root(0), dict(), \
forward(this, &lexer::emit ),\
discard(this, &lexer::drop )
        
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
            lexical::scanner::pointer ptr( new lexical::scanner(id,line,dict) );
            ptr->link_to(*this);
            
            if( !scanners.insert(ptr) )
                throw exception("lexer {%s}: multiple scanner <%s>", name.c_str(), id.c_str());
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
            last_label.clear();
            last_token.clear();
        }
        
        void lexer::jump(const string &id)
        {
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan) throw exception("{%s}.jump(NO <%s>)", name.c_str(), id.c_str());
            scan = & (**ppScan);
        }
        
        void lexer:: call( const string &id )
        {
            assert(scan);
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(!ppScan) throw exception("{%s}.call(NO <%s>)", name.c_str(), id.c_str());
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
        
        
        lexeme * lexer:: get(source &src, ios::istream &fp)
        {
            if(!scan)
                throw exception("{%s} has no active scanner", name.c_str());
            
            lexeme *lx = 0;
            if(cache.size>0)
            {
                lx = cache.pop_front();
                goto RETURN_LEXEME;
            }
            else
            {
                bool is_control  = false;
                while(true)
                {
                    lx = scan->get(src, fp, is_control);
                    if(!lx)
                    {
                        if(!is_control)
                        {
                            // EOF
                            return 0;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    assert(!is_control);
                    goto RETURN_LEXEME;
                }
            }
            
        RETURN_LEXEME:
            assert(lx);
            try
            {
                last_label = lx->label;
                last_token.clear();
                for(const t_char *ch=lx->head;ch;ch=ch->next)
                {
                    last_token.append(ch->code);
                }
                return lx;
            }
            catch(...)
            {
                cache.push_front(lx);
                throw;
            }
        }
        
        void lexer:: unget(lexeme *lx ) throw()
        {
            assert(lx!=0);
            cache.push_front(lx);
        }
        
        const lexical::scanner & lexer:: current() const throw()
        {
            assert(scan);
            return *scan;
        }
        
        
    }
}
