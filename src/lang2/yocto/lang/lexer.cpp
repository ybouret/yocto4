#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        lexer:: ~lexer() throw() {}

#define Y_LANG_LEXER_CTOR() \
name(id),                   \
line(1),                    \
scan(0),                    \
Root(0),                    \
scanners(2,as_capacity),    \
history(),                  \
dict()


        lexer:: lexer(const string &id, const string &root_scanner) :
        Y_LANG_LEXER_CTOR()
        {
            initialize(root_scanner);
        }

        void lexer:: initialize(const string &root_scanner)
        {
            Root = new lexical::scanner(root_scanner,line);
            const p_scanner q(Root);
            if(!scanners.insert(q))
                throw exception("{%s} unexpected scanner<%s> database failure !", name.c_str(), Root->name.c_str());
            scan = Root;
            Root->link_to(*this);
        }

        lexer:: lexer(const char   *id, const char *root_scanner) :
        Y_LANG_LEXER_CTOR()
        {
            const string rs(root_scanner);
            initialize(rs);
        }


        lexical::scanner & lexer:: get_root() throw()
        {
            assert(Root);
            return *Root;
        }

        lexical::scanner & lexer:: declare(const string &id)
        {
            p_scanner *pp = scanners.search(id);
            if(pp)
                throw exception("{%s}: multiple scanned <%s>", name.c_str(), id.c_str());

            p_scanner q( new lexical::scanner(id,line) );
            if(! scanners.insert(q) )
            {
                throw exception("{%s}[<%s>]: unexpected creation failure", name.c_str(), id.c_str());
            }
            
            q->link_to(*this);
            return *q;
        }

        lexical::scanner & lexer:: declare(const char *txt)
        {
            const string id(txt);
            return declare(id);
        }


        void lexer:: restart() throw()
        {
            scan = Root;
            line = 1;
            history.clear();
        }
    }

}


namespace yocto
{
    namespace lang
    {

        void lexer:: jump(const string &id)
        {
            p_scanner *pp = scanners.search(id);
            if(!pp)
            {
                throw exception("{%s}: no <%s> to JUMP", name.c_str(), id.c_str());
            }
            lexical::scanner &s = **pp;
            scan = &s;
            std::cerr << "\t@JUMP@" << id << std::endl;
        }

        void lexer:: call(const string &id)
        {
            p_scanner *pp = scanners.search(id);
            if(!pp)
            {
                throw exception("{%s}: no <%s> to JUMP", name.c_str(), id.c_str());
            }
            lexical::scanner &s = **pp;
            history.append(scan);
            scan = &s;
            std::cerr << "\t@CALL@" << id << std::endl;
        }

        void lexer:: back()
        {
            if(history.size<=0) throw exception("{%s}: no previous CALL to be BACK...", name.c_str());
            assert(0!=history.tail->addr);
            scan = history.tail->addr;
            history.remove();
            std::cerr << "\t@BACK@" << scan->name << std::endl;
        }


    }

}


namespace yocto
{
    namespace lang
    {
        lexeme *lexer:: read_from(source &src, ios::istream &fp)
        {
            assert(scan);
            for(;;)
            {
                bool    ctrl = false;
                lexeme *lx   = scan->get(src, fp, ctrl);
                if(!lx)
                {
                    if(!ctrl)
                    {
                        // EOF
                        return 0;
                    }
                    else
                    {
                        // it was a control lexeme
                        continue;
                    }
                }
                return lx;
            }

        }


        lexeme * lexer:: get( source &src, ios::istream &fp )
        {
            assert(scan);
            if(size>0)
            {
                return pop_front();
            }
            else
            {
                return read_from(src,fp);
            }
        }

        void   lexer:: unget( lexeme *lx ) throw()
        {
            assert(lx);
            push_front(lx);
        }

        void    lexer:: uncpy( const lexeme *lx )
        {
            assert(lx);
            push_front( new lexeme(*lx) );
        }


        const lexeme * lexer:: peek(source &src, ios::istream &fp)
        {
            if(size>0)
            {
                return head;
            }
            else
            {
                lexeme *lx = read_from(src, fp);
                if(lx) { push_front(lx); assert(1==size); }
                return head;
            }
        }
        
        
        
    }
}


