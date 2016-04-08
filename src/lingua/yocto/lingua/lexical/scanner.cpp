#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }

            const string & scanner:: key() const throw()
            {
                return name;
            }


#define Y_SCANNER_CTOR() , lex(0)

            scanner:: scanner(const string &id, int &line_ref) :
            name(id),
            line(line_ref) Y_SCANNER_CTOR()
            {
            }


            scanner:: scanner(const char *id, int &line_ref) :
            name(id),
            line(line_ref) Y_SCANNER_CTOR()
            {
            }


            void scanner:: check(const string &label)
            {
                if(label.size()<=0) throw exception("scanner: empty rule label");

                for(const rule *r=rules.head;r;r=r->next)
                {
                    if(label==r->label) throw exception("scanner: multiple rule '%s'", label.c_str());
                }

            }

        }
    }
}

#include "yocto/lingua/pattern/posix.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            bool scanner:: forward(const token&) throw() { return true;  }
            bool scanner:: discard(const token&) throw() { return false; }
            bool scanner:: newline(const token&) throw() { ++line; return false; }

            void scanner::make(const string &l,pattern *p,const action &a)
            {
                const pattern::ptr q(p);
                check(l);
                rules.push_back( new rule(l,q,a,false) );
            }

            void scanner:: emit(const string &label, const string &expr)
            {
                make(label,expr,this, &scanner::forward);
            }

            void scanner:: emit(const char *label, const char *expr)
            {
                make(label,expr,this, &scanner::forward);
            }

            void scanner:: drop(const string &label, const string &expr)
            {
                make(label,expr,this, &scanner::discard);
            }

            void scanner:: drop(const char *label, const char *expr)
            {
                make(label,expr,this, &scanner::discard);
            }
            

            void scanner:: endl(const string &label)
            {
                const action a(this,&scanner::newline);
                make(label, posix::endl(), a);
            }

            void scanner:: endl(const char *label)
            {
                const string l(label);
                const action a(this,&scanner::newline);
                make(l, posix::endl(), a);
            }

        }

    }
}
