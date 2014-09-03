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
        namespace lexical
        {
            
            
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
                
                //--------------------------------------------------------------
                //
                // make rules: simple undertaken actions
                //
                //--------------------------------------------------------------
                void make(const string  &label,
                          const string  &regex,
                          const action  &cb );
                
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
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list  rules;
                p_dict *dict_;
                
                
            public:
                bool echo;
                
            };
            
        }
    }
}

#endif
