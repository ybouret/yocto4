#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        bool  lexer:: emit(const token &) throw() { return true;  }
        bool  lexer:: drop(const token &) throw() { return false; }
        void  lexer:: on_newline(const token &) throw() { ++line; }
        bool  lexer:: on_newline_drop(const token&t) throw() { on_newline(t); return false; }
        bool  lexer:: on_newline_emit(const token&t) throw() { on_newline(t); return true;  }
		void  lexer:: do_nothing(const token &) throw() {}
        
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
        
        lexical::scanner & lexer:: operator[](const string &id)
        {
            lexical::scanner::pointer *ppScan = scanners.search(id);
            if(ppScan)
            {
                return **ppScan;
            }
            else
            {
                return declare(id);
            }
        }
        
        lexical::scanner & lexer:: operator[](const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        lexer:: ~lexer() throw()
        {
        }
        
#if defined(_MSC_VER)
#pragma warning ( disable : 4355 )
#endif
        
#define Y_LEXER_CTOR() \
name(id), line(1), scan(0), cache(), history(),\
last_label(),last_token(),last_line(0),\
scanners(2,as_capacity), root(0),\
dict(), \
forward(this, &lexer::emit ),\
discard(this, &lexer::drop ),\
newline(this, &lexer::on_newline_drop ),\
noop_cb(this, &lexer::do_nothing),\
endl_cb(this, &lexer::on_newline)
        
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
        
        bool lexer:: has(const char   *id) const
        {
            const string ID(id);
            return has(ID);
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
            last_line = 0;
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
            //__________________________________________________________________
            //
            // scan cache or input
            //__________________________________________________________________
        NEXT_LEXEME:
            if(cache.size>0)
            {
                //______________________________________________________________
                //
                // least effort...
                //______________________________________________________________
                lx = cache.pop_front();
                goto RETURN_LEXEME;
            }
            else
            {
                //______________________________________________________________
                //
                // probe input
                //______________________________________________________________
                bool is_control  = false;
                while(true)
                {
                    lx = scan->get(src, fp, is_control);
                    if(!lx)
                    {
                        if(!is_control)
                        {
                            //__________________________________________________
                            //
                            // EOF
                            //__________________________________________________
                            return 0;
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // a control lexeme may produce a real lexeme !
                            //__________________________________________________
                            goto NEXT_LEXEME;
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
                last_line = lx->line;
                return lx;
            }
            catch(...)
            {
                cache.push_front(lx);
                throw;
            }
        }
        
        const lexeme * lexer:: peek(source &src,ios::istream&fp)
        {
            if(cache.size>0)
            {
                return cache.head;
            }
            else
            {
                lexeme *lx = get(src, fp);
                if(!lx)
                {
                    return 0;
                }
                else
                {
                    cache.push_front(lx);
                    assert(1==cache.size);
                    return cache.head;
                }
            }
        }
        
        
        void lexer:: unget(lexeme *lx ) throw()
        {
            assert(lx!=0);
            cache.push_front(lx);
        }
        
        void lexer:: unget( const lexical::scanner &parent, const string &content)
        {
            lexeme *lx = new lexeme(parent.name,line);
            try
            {
                *static_cast<token *>(lx) = content;
            }
            catch(...)
            {
                delete lx;
                throw;
            }
            unget(lx);
            
        }
        
        const lexical::scanner & lexer:: current() const throw()
        {
            assert(scan);
            return *scan;
        }
        
              
    }
}
