#ifndef YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LINGUA_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lingua/lexical/rule.hpp"
#include "yocto/lingua/pattern/regexp.hpp"
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

#define YOCTO_LINGUA_SCANNER_DEF(METH,FUNC) \
inline void METH(const string &label, const string &expr) { make(label,expr,this, &scanner:: FUNC); } \
inline void METH(const char   *label, const char   *expr) { make(label,expr,this, &scanner:: FUNC); }

                YOCTO_LINGUA_SCANNER_DEF(emit,forward)
                YOCTO_LINGUA_SCANNER_DEF(drop,discard)
                void endl(const string &label);
                void endl(const char   *label);

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
