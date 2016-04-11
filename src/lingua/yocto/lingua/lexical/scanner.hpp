#ifndef YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lingua/lexical/rule.hpp"
#include "yocto/lingua/pattern/regexp.hpp"
#include "yocto/lingua/lexeme.hpp"

#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace lingua
    {
        class lexer;

        namespace lexical
        {

            class scanner : public counted_object
            {
            public:
                typedef intr_ptr<string,scanner> ptr;

                const string name; //!< scanner name
                int         &line; //!< line reference

                const string &key() const throw();
                virtual ~scanner() throw();

                explicit scanner(const char   *id, int &line_ref);
                explicit scanner(const string &id, int &line_ref);


                //! action upon pattern
                void make(const string &l,pattern *p,const action &a);

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void make(const string  &label,
                                 const string  &expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const action a(host,method);
                    make(label, regexp(expr,dict), a);
                }

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void make(const char    *label,
                                 const char    *expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const string l(label);
                    const action a(host,method);
                    make(l, regexp(expr,dict), a);
                }


                bool forward(const token&) throw(); //!< return true
                bool discard(const token&) throw(); //!< return false
                bool newline(const token&) throw(); //!< increase line, return false

                void emit(const string &label, const string &expr); //!< will produce token
                void emit(const char   *label, const char   *expr); //!< will produce token

                void drop(const string &label, const string &expr); //!< will discard token
                void drop(const char   *label, const char   *expr); //!< will discard token

                void endl(const string &label); //!< silent endl
                void endl(const char   *label); //!< silent endl

                //! get next lexeme or control flag
                lexeme *get( source &src, bool &ctrl);



            protected:
                lexer  *lex;  //!< the line ref must match !
                p_dict *dict; //!< available if attached to lexer
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                void check(const string &label);
                r_list rules;
            };
            
        }
    }
}

#endif
