#ifndef YOCTO_LANG_SCANNER_INCLUDED
#define YOCTO_LANG_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/rx/pattern/dict.hpp"

namespace yocto 
{
    namespace lang
    {
        
        namespace lexical
        {
            
            
            class scanner 
            {
            public:
                explicit scanner( const string &id);
                virtual ~scanner() throw();
                
                const string name; //!< scanner identifier
                size_t       line; //!< line index for lexemes
                
            private:
                rules         rules_;
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                bool __forward( const regex::token &) throw(); //!< return true
                bool __discard( const regex::token &) throw(); //!< return false
                bool __newline( const regex::token &) throw(); //!< increase line, return true

            public:
                const action  forward; //!< predefined action
                const action  discard; //!< predefined action
                const action  newline; //!< predefined action
                
                //! create a new rule with label, motif and optional proc (forward if NULL==proc)
                void operator()( const string &label, regex::pattern *motif, const action *proc = NULL );
                
                //! create a new rule with the compiled expr, use internal dict
                void operator()( const string &label, const string &expr, const action *proc =NULL);
                
                //! create a new rule with the compiled expr, use internal dict
                void operator()( const char *label, const char *expr, const action *proc = NULL );
                
                template <typename  HOST>
                void on( const char *label, const char *expr, HOST *host, bool (HOST::*method)( const regex::token &) )
                {
                    assert(host); 
                    assert(method);
                    const action cb( host, method );
                    (*this)(label,expr,&cb);
                }
                
                
                //! create dict if necessary
                regex::pattern_dict &dict();
                
                //! release dictionary
                void                 no_dict() throw();
                
            private:
                regex::pattern_dict *dict_;
            };
        }
        
        
    }
}

#endif
