#ifndef YOCTO_LANG_SCANNER_INCLUDED
#define YOCTO_LANG_SCANNER_INCLUDED 1

#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"
#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/rx/pattern/dict.hpp"
#include "yocto/rx/source.hpp"

namespace yocto 
{
    namespace lang
    {
        
        class lexer;
        
        namespace lexical
        {
            
            typedef functor<void,TL1(const regex::token &)> callback;
            
            //! a simple scanner
            class scanner : public object, public counted
            {
            public:
                explicit scanner( const string &id, size_t &line_ref);
                explicit scanner( const char   *id, size_t &line_ref);
                virtual ~scanner() throw();
                
                const string name; //!< scanner identifier
                size_t      &line; //!< line index for lexemes
                
                
            private:
                rules         rules_;
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                bool __forward( const regex::token &) throw(); //!< return true
                bool __discard( const regex::token &) throw(); //!< return false
                bool __newline( const regex::token &) throw(); //!< increase line, return true
                bool __no_endl( const regex::token &) throw(); //!< increase line, return false
                
            public:
                const action  forward; //!< predefined action : forward lexeme (return true), DEFAULT
                const action  discard; //!< predefined action : discard lexeme (return false)
                const action  newline; //!< predefined action : increase line and RETURN lexeme (return true)
                const action  no_endl; //!< predefined action : increase line and DISCARD lexeme (return false)
                
                //==============================================================
                // lexemes recognition
                //==============================================================
                
                //! create a new rule with label, motif and optional proc (forward if NULL==proc)
                void make( const string &label, regex::pattern *motif, const action *proc = NULL );
                
                //! create a new rule with the compiled expr, use internal dict
                void make( const string &label, const string &expr, const action *proc =NULL);
                
                //! create a new rule with the compiled expr, use internal dict
                void make( const char *label, const char *expr, const action *proc = NULL );
                
                template <typename  HOST>
                void make( const char *label, const char *expr, HOST *host, bool (HOST::*method)( const regex::token &) )
                {
                    assert(host); 
                    assert(method);
                    const action cb( host, method );
                    make(label,expr,&cb);
                }
                
                //==============================================================
                // jump within lexer's sub-scanners
                //==============================================================
                void jump( const string &id, regex::pattern *motif, const callback &cb );
                void jump( const string &id, const string &expr, const callback &cb );
                void jump( const char   *id, const char   *expr, const callback &cb );
                template <typename  HOST>
                void jump( const char *id, const char *expr, HOST *host, void (HOST:: *method)(const regex::token &) )
                {
                    assert(host); 
                    assert(method);
                    const callback cb( host, method );
                    jump(id,expr,cb);
                }
                
                //==============================================================
                // call one of the lexer's sub-scanners
                //==============================================================
                void call( const string &id, regex::pattern *motif, const callback &cb );
                void call( const string &id, const string   &expr,  const callback &cb );
                void call( const char   *id, const char     *expr,  const callback &cb );
                template <typename  HOST>
                void call( const char *id, const char *expr, HOST *host, void (HOST:: *method)(const regex::token &) )
                {
                    assert(host); 
                    assert(method);
                    const callback cb( host, method );
                    call(id,expr,cb);
                }
                
                //==============================================================
                // back to the calling lexer's scanners
                //==============================================================
                void back( regex::pattern *motif, const callback &cb );
                void back( const string   &expr,  const callback &cb );
                void back( const char     *expr,  const callback &cb );
                template <typename  HOST>
                void back( const char *expr, HOST *host, void (HOST:: *method)(const regex::token &) )
                {
                    assert(host); 
                    assert(method);
                    const callback cb( host, method );
                    back(expr,cb);
                }
                
                //! create dict if necessary
                regex::pattern_dict &dict();
                
                //! release dictionary
                void                 no_dict() throw();
                
                //! reset all rules/motif
                void reset() throw();
                
                //! get next forwarded lexeme
                lexeme * next_lexeme( regex::source &src, bool &fctl);
                
                //! get key for set of scanner
                const string & key() const throw();
                
                //! counted smart pointer
                typedef  intr_ptr<string,scanner> ptr;
                
                //! set parent
                void link_to( lexer &parent ) throw();
                
                
            private:
                lexer               *lexer_;
                regex::pattern_dict *dict_;
                unsigned             opid_;   //!< to create jump/call/back label
            };
        }
        
        
    }
}

#endif
