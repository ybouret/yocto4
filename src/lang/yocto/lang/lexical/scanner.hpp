#ifndef YOCTO_LANG_SCANNER_INCLUDED
#define YOCTO_LANG_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/lang/pattern/dict.hpp"

namespace yocto
{
    namespace lang
    {
        class lexer;
        
        namespace lexical
        {
            
            typedef functor<void,TL1(const token &)> callback;
            
            class scanner : public object, public counted
            {
            public:
                typedef intr_ptr<string,scanner> pointer;
                
                //! default ctor
                explicit scanner( const string &id, int &line_ref );
                
                //! default ctor
                explicit scanner( const char   *id, int &line_ref );
                virtual ~scanner() throw();
                
                const string name; //!< its name for lexer
                int         &line; //!< a line number reference
                
                const string & key() const throw(); //!< return the name
                
                void reset() throw(); //!< reset all motif from content
                
                //! append a rule while checking name, delete upon error
                void append( rule *r );
                
                //! create/return a dictionary if needed
                p_dict & dict();
                
                //! delete the dictionary
                void     no_dict() throw();
                
                
                //! lexeme scanning protocol
                /**
                 find the first longest matching pattern and make
                 a lexeme out of it.
                 - if returns != NULL, a valid lexeme
                 - if returns NULL:
                 -- if is_control==false => EOF
                 -- if is_control==true  => control rule was met
                 */
                lexeme *get( source &src, ios::istream &fp, bool &is_control );
                
                //! link to a lexer
                void link_to( lexer & );
                
                
                //--------------------------------------------------------------
                //
                // make rules: simple undertaken actions
                //
                //--------------------------------------------------------------
                void make(const string  &label,
                          const string  &regex,
                          const action  &todo );
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const string   &label,
                          const string   &regex,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  method)
                {
                    const action cb(host,method);
                    make(label,regex,cb);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const char     *label,
                          const char     *regex,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  method)
                {
                    const string L(label);
                    const string R(regex);
                    make(L,R,host,method);
                }
                
                void forward(const string &label, const string &regex);
                void forward(const char   *label, const char   *regex);
                
                void discard(const string &label, const string &regex);
                void discard(const char   *label, const char   *regex);
                
                
                bool    emit( const token & );
                bool    drop(const token & );
                bool    newline(const token &);        //!< increase #line, discard
                bool    newline_emit(const token &);   //!< increase #line, forward
                
                //--------------------------------------------------------------
                //
                // jump: jumping to a lexer's subscanner
                //
                //--------------------------------------------------------------
                
                //! change lexer's scanner with a jump
                void jump(const string   &target,
                          const string   &regex,
                          const callback &onJump);
                
                
                //--------------------------------------------------------------
                //
                // call: calling a lexer's subscanner
                //
                //--------------------------------------------------------------
                
                //! change lexer's scanner with a call
                void call(const string   &target,
                          const string   &regex,
                          const callback &onJump);
                
                
                //--------------------------------------------------------------
                //
                // back: returning to a lexer's calling subscanner
                //
                //--------------------------------------------------------------
                void back(const string   &regex,
                          const callback &onBack);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list  rules;
                lexer  *mylex;
                p_dict *dict_;
                
                void ctrl(const string  &label,
                          const string  &regex,
                          const action  &todo );
                
                void __build(const string  &label,
                             const string  &regex,
                             const action  &todo,
                             const bool     is_control );
                
            public:
                bool echo;
                
            private:
                int iBack;
                
            };
            
        }
    }
}

#endif
