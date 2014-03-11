#ifndef YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lingua/lexeme.hpp"
#include "yocto/lingua/lexical/rules.hpp"
#include "yocto/lingua/pattern/compiler.hpp"

#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        class lexer;
        
        namespace lexical
        {
            //__________________________________________________________________
            //
            //! control callback
            //__________________________________________________________________
            typedef  functor<void,TL1(const token &)> callback;
            
            //__________________________________________________________________
            //
            //! tunable lexical scanner
            //__________________________________________________________________
            class scanner : public object, public counted
            {
            public:
                //! ctor: scanner ID + a ref to a line count
                explicit scanner( const string &id, size_t &line_ref);
                explicit scanner( const char   *id, size_t &line_ref);
                
                virtual ~scanner() throw();
                
                const string name;
                size_t      &line; //!< reference to external line number
                
                const string & key() const throw();
                
                typedef intr_ptr<string,scanner> ptr;
                
                //! search for a rule label
                bool has( const string &id ) const throw();
                
                //==============================================================
                //
                // some helpers for lexical action
                //
                //==============================================================
                
                //! method pointer prototype
                typedef bool (scanner::*method)(const token &);

                //! return true
                bool __forward( const token & ) throw();
                
                //! return false
                bool __discard( const token & ) throw();
                
                //! increase line and return true (forward newline)
                bool __newline( const token & ) throw();
                
                //! increase line and return false (discard newline)
                bool __no_endl( const token & ) throw();
                
                
                inline method forward() const throw()
                {
                    return &scanner:: __forward;
                }
                
                inline method discard() const throw()
                {
                    return &scanner:: __discard;
                }
                
                inline method newline() const throw()
                {
                    return &scanner:: __newline;
                }
                
                inline method no_endl() const throw()
                {
                    return &scanner:: __no_endl;
                }
                                
                //==============================================================
                //
                // lexeme creation
                //
                //==============================================================
                void make(const string &label,
                          pattern      *motif,
                          const action &cb);
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void make(const string  &label,
                                 const string  &expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    assert(host);
                    assert(meth);
                    const action cb(host,meth);
                    pattern     *motif = compile(expr,pdict.__get());
                    make(label,motif,cb);
                }
                
                
                template <typename OBJECT_POINTER,typename OBJECT_METHOD>
                void make(const char    *label,
                          const char    *expr,
                          OBJECT_POINTER host,
                          OBJECT_METHOD  meth)
                {
                    const string L(label);
                    const string E(expr);
                    make<OBJECT_POINTER,OBJECT_METHOD>(L,E,host,meth);
                }
                
                //==============================================================
                //
                // jump to my lexer's sub-scanner
                //
                //==============================================================
                void jump(const string   &id,
                          pattern        *motif,
                          const callback &onJump );
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void jump(const string  &label,
                                 const string  &expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    assert(host);
                    assert(meth);
                    const callback onJump(host,meth);
                    pattern       *motif = compile(expr,pdict.__get());
                    jump(label,motif,onJump);
                }
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void jump(const char *label,
                                 const char  *expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    const string L(label);
                    const string E(expr);
                    jump<OBJECT_POINTER,OBJECT_METHOD>(L,E,host,meth);
                }
                
                //==============================================================
                //
                // call my lexer's sub-scanner
                //
                //==============================================================
                void call(const string   &id,
                          pattern        *motif,
                          const callback &onCall );
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void call(const string  &label,
                                 const string  &expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    assert(host);
                    assert(meth);
                    const callback onCall(host,meth);
                    pattern       *motif = compile(expr,pdict.__get());
                    call(label,motif,onCall);
                }
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void call(const char *label,
                                 const char  *expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    const string L(label);
                    const string E(expr);
                    call<OBJECT_POINTER,OBJECT_METHOD>(L,E,host,meth);
                }
                
                
                //==============================================================
                //
                // back from a previous call
                //
                //==============================================================
                void back( pattern *motif, const callback &onBack );
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void back(const string  &expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    assert(host);
                    assert(meth);
                    const callback onBack(host,meth);
                    pattern       *motif = compile(expr,pdict.__get());
                    back(motif,onBack);
                }
                
                template <typename OBJECT_POINTER, typename OBJECT_METHOD>
                inline void back(const char    *expr,
                                 OBJECT_POINTER host,
                                 OBJECT_METHOD  meth)
                {
                    const string E(expr);
                    back<OBJECT_POINTER,OBJECT_METHOD>(E,host,meth);
                }
                
                
                //==============================================================
                //
                // dictionary
                //
                //==============================================================
                p_dict & dict();
                void     no_dict() throw();
                
                //==============================================================
                //
                // run
                //
                //==============================================================
                
                //! reset all rules/motif and set line to 1
                void reset() throw();
                
                //! try to get next lexeme
                /**
                 throw an exception if no match.
                 \return 
                 - if not NULL, a lexeme, and fctl = false
                 - if NULL: 
                 -- fctl = false => EOF
                 -- fctl = true  => jump/call/back, change of active scanner for lexer
                 */
                lexeme *get( source &src , bool &fctl );
                
                void link_to( lexer & ) throw();
                
                //! simulate a lexeme recognition
                void emit( lexer &, const string & ) const;
                
                //! simulate a lexeme recognition
                void emit( lexer &, token &) const;
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list           rules;
                lexer           *mylex;
                auto_ptr<p_dict> pdict;
                unsigned         opsID;
            };
            
#define Y_LEX_FORWARD(REF,ID,EXPR) ((REF).make(ID,EXPR, &(REF), (REF).forward()))
#define Y_LEX_DISCARD(REF,ID,EXPR) ((REF).make(ID,EXPR, &(REF), (REF).discard()))
#define Y_LEX_NO_ENDL(REF)         ((REF).make("end","[:endl:]", &(REF), (REF).no_endl()))
            
            
        }
    }
    
}

#endif
