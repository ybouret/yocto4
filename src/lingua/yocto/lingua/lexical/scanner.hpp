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

            typedef functor<void,TL1(const token &)> callback;

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


                //! lexical action upon pattern
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
                void endl_cb(const token&) throw(); //!< increase line callback
                void drop_cb(const token&) throw(); //!< do nothing callback
                
                void emit(const string &label, const string &expr); //!< will produce token
                void emit(const char   *label, const char   *expr); //!< will produce token

                void drop(const string &label, const string &expr); //!< will discard token
                void drop(const char   *label, const char   *expr); //!< will discard token

                void endl(const string &label); //!< silent endl
                void endl(const char   *label); //!< silent endl

                //! get next lexeme or control flag
                /**
                 first longest matching...
                 - return a regular lexeme (ctrl==false)
                 - or return NULL: if(ctrl==true) => for lexer, if(ctrl==false)=> End of Source...
                 */
                lexeme *get( source &src, bool &ctrl);

                //! link to a parent lexer, from where 'line' comes..
                void link_to( lexer &parent) throw();

                
                void stop(const string &expr); //!< will stop lexer upon expression


                //! will call lexer 'call(id)' and execute callback upon expr
                void call(const string   &id,
                          const string   &expr,
                          const callback &cb);

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void call(const string  &id,
                                 const string  &expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const callback cb(host,method);
                    call(id,expr,cb);
                }

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void call(const char    *id,
                                 const char    *expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const string ID(id);
                    const string EXPR(expr);
                    const callback cb(host,method);
                    call(ID,EXPR,cb);
                }



                void back(const string   &expr,
                          const callback &cb);


                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void back(const string  &expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const callback cb(host,method);
                    back(expr,cb);
                }

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void back(const char    *expr,
                                 OBJECT_POINTER host,
                                 METHOD_POINTER method)
                {
                    const string EXPR(expr);
                    const callback cb(host,method);
                    back(EXPR,cb);
                }




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
