#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/lang/pattern/compiler.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
            void scanner:: __build(const string    &label,
                                   const string    &regex,
                                   const action    &todo,
                                   const rule::kind flag )
            {
                pattern *p = compile(regex,dict_);
                rule    *r = rule::create(label,p,todo,flag);
                append( r );
            }
            
            void scanner:: make(const string &label,
                                const string &regex,
                                const action &todo)
            {
               
                __build(label, regex, todo, rule::is_regular);
            }
            
            void scanner:: ctrl(const string &label,
                                const string &regex,
                                const action &todo)
            {
                __build(label, regex, todo, rule::is_control);
            }
            
            
            
            void scanner:: forward(const string &label, const string &regex)
            {
                this->make(label,regex,this, & scanner::emit );
            }
            
            void scanner:: forward(const char *label, const char *regex)
            {
                const string L(label);
                const string R(regex);
                
                forward(L,R);
                
            }
            
            bool scanner:: emit( const token &tkn )
            {
                if(echo) std::cerr << tkn << std::endl;
                return true;
            }
            
            void scanner:: discard(const string &label, const string &regex)
            {
                this->make(label,regex,this, & scanner::drop );
            }
            
            void scanner:: discard(const char *label, const char *regex)
            {
                const string L(label);
                const string R(regex);
                
                discard(L,R);
            }
            
            
            bool scanner:: drop(const token &tkn )
            {
                if(echo) std::cerr << tkn << std::endl;
                return false;
            }
            
            bool scanner:: newline(const token &)
            {
                ++line;
                return false;
            }
            
            bool scanner:: newline_emit(const token &)
            {
                ++line;
                return true;
            }
        }
    }
}
