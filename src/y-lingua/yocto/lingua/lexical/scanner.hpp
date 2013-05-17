#ifndef YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lingua/lexeme.hpp"
#include "yocto/lingua/lexical/rules.hpp"
#include "yocto/lingua/pattern/compiler.hpp"

#include "yocto/intrusive-ptr.hpp"
#include "yocto/counted.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        class lexer;
        
        namespace lexical
        {
            
            typedef  functor<void,TL1(const token &)> callback;
            
            class scanner : public object, public counted
            {
            public:
                explicit scanner( const string &id, size_t &line_ref);
                explicit scanner( const char   *id, size_t &line_ref);
                
                virtual ~scanner() throw();
                
                const string name;
                size_t      &line; //!< reference to external line number
                
                const string & key() const throw();
                
                typedef intrusive_ptr<string,scanner> ptr;
                bool has( const string &id ) const throw();
                
                //==============================================================
                //
                // some helper callbacks
                //
                //==============================================================
                
                //! return true
                bool __forward( const token & ) throw();
                
                //! return false
                bool __discard( const token & ) throw();
                
                //! increase line and return true (forward newline)
                bool __newline( const token & ) throw();
                
                //! increase line and return false (discard newline)
                bool __no_endl( const token & ) throw();
                
                typedef bool (scanner::*method)(const token &);
                
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
                
                //==============================================================
                //
                // call my lexer's sub-scanner
                //
                //==============================================================
                void call(const string   &id,
                          pattern        *motif,
                          const callback &onCall );
                
                
                //==============================================================
                //
                // back from a previous call
                //
                //==============================================================
                void back( pattern *motif, const callback &onBack );
                
                
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
                 \return - if not NULL, a lexeme, and fctl = false
                 - if NULL: fctl = false => EOF
                 fctl = true  => jump/call/back, change of active scanner for lexer
                 */
                lexeme *get( source &src , bool &fctl );
                
                void link_to( lexer & ) throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list           rules;
                lexer           *mylex;
                auto_ptr<p_dict> pdict;
                unsigned         opsID;
            };
            
#define Y_LEX_FORWARD(REF,ID,EXPR) (REF).make(ID,EXPR, &(REF), (REF).forward())
#define Y_LEX_DISCARD(REF,ID,EXPR) (REF).make(ID,EXPR, &(REF), (REF).discard())
            
        }
    }
    
}

#endif
