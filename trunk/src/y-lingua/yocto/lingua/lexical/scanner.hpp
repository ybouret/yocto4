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
        
        namespace lexical
        {
            
            class scanner : public object, public counted
            {
            public:
                explicit scanner( const string &id, size_t &line_ref);
                virtual ~scanner() throw();
                
                const string name;
                size_t      &line; //!< reference to external line number
                
                const string & key() const throw();
                
                typedef intrusive_ptr<string,scanner> ptr;
                
                //==============================================================
                //
                // make action upon pattern
                //
                //==============================================================
                void make(const string &label,
                          pattern      *motif,
                          const action &cb);
                
                template <typename OBJECT_POINTER,typename OBJECT_METHOD>
                void make(const string  &label,
                          const string  &expr,
                          OBJECT_POINTER host,
                          OBJECT_METHOD  meth)
                {
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
                // some callbacks
                //
                //==============================================================
                
                //! return true
                bool forward( const token & ) const throw();
                
                //! return false
                bool discard( const token & ) const throw();
                
                //! increase line and return true (forward newline)
                bool newline( const token & ) throw();
                
                //! increase line and return false (discard newline)
                bool no_endl( const token & ) throw();
                
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
                
                //! reset all rules/motif and set line to 0
                void reset() throw();
                
                //! try to get next lexeme
                /**
                 throw an exception if no match.
                 \return - if not NULL, a lexeme, and fctl = false
                         - if NULL: fctl = false => EOF
                                    fctl = true  => jump/call/back, change of active scanner for lexer
                 */
                lexeme *next_lexeme( source &src , bool &fctl );
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list           rules;
                auto_ptr<p_dict> pdict;
            };
            
        }
    }
    
}

#endif
