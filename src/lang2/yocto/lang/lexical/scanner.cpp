#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            scanner:: ~scanner() throw() {}

#define Y_LANG_SCANNER_CTOR() \
name(id),   \
line(ir),   \
rules(),    \
cache(),    \
echo(false)

            scanner:: scanner(const string &id , int &ir) :
            Y_LANG_SCANNER_CTOR()
            {
            }

            scanner:: scanner(const char *id , int &ir) :
            Y_LANG_SCANNER_CTOR()
            {
            }


            void scanner:: check_label(const string &label)
            {
                for(const rule *r=rules.head;r;r=r->next)
                {
                    if(r->label==label)
                    {
                        throw exception("multiple rule '%s' in <%s>", label.c_str(), name.c_str());
                    }
                }
            }

            bool scanner:: forward(const token &tkn)
            {
                if(echo)
                {
                    std::cerr << line << ":<" << name << " > => +" << tkn << std::endl;
                }
                return true;
            }

            bool scanner:: discard(const token &)
            {
                if(echo)
                {

                }
                return false;
            }

            bool scanner:: newline(const token &)
            {
                if(echo)
                {
                    std::cerr << "<" << name << ">" << " ENDL #" << line << std::endl;
                }
                ++line;
                return false;
            }

            const string & scanner:: key() const throw() { return name; }
        }
    }

}

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            void  scanner:: make(const string &label, pattern *p, const action &a)
            {
                auto_ptr<pattern> q(p);
                check_label(label);
                rules.push_back( rule::create(label,q.yield(),a) );
            }

            void  scanner:: make(const char *l, const char *expr, const action &a, const p_dict *dict)
            {
                const string label(l);
                make(label, regexp(expr,dict), a);
            }

            void scanner:: emit(const char *label, const char *expr,const p_dict *dict)
            {
                make(label,expr,this, & scanner::forward, dict);
            }

            void scanner:: drop(const char *label, const char *expr,const p_dict *dict)
            {
                make(label,expr,this, & scanner::discard, dict);
            }
            
            void scanner:: endl(const char *label)
            {
                make(label,"[:endl:]", this, &scanner::newline, NULL);
            }
            
        }
        
    }
}
