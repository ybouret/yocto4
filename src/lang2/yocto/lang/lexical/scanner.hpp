#ifndef YOCTO_LANG_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LANG_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/lang/regexp.hpp"
#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace lang
    {

        class lexer;

        namespace lexical
        {

            //! a callback for control rules
            typedef functor<void,TL1(const token&)> callback;
            

            //! a simple scanner
            /**
             Transform a source of t_char into a source of lexemes.
             */
            class scanner : public counted_object
            {
            public:
                typedef intr_ptr<string,scanner> ptr;
                virtual ~scanner() throw();
                explicit scanner(const string &id, int &ir);
                explicit scanner(const char   *id, int &ir);

                const string &key() const throw();
                
                const string name;  //!< identifier
                int         &line;  //!< line index reference

                void link_to( lexer &parent );

                //--------------------------------------------------------------
                //
                // rule: producing lexemes
                //
                //--------------------------------------------------------------

                //! action upon pattern
                void  make(const string &label, pattern *p, const action &a);

                //! action upon pattern, wrapper
                void  make(const char   *label, const char *expr, const action &a, const p_dict *dict=NULL);

                //! action upon pattern, wrapper
                template <typename HOST_POINTER, typename METHOD_POINTER>
                inline void make(const char *label, const char *expr, HOST_POINTER h, METHOD_POINTER m, const p_dict *dict=NULL)
                {
                    const action a(h,m);
                    make(label,expr,a,dict);
                }

                //! action= forward
                void emit(const char *label, const char *expr,const p_dict *dict=NULL);

                //! action= discard
                void drop(const char *label, const char *expr,const p_dict *dict=NULL);

                //! action= newline
                void endl(const char *label);


                bool forward( const token & ); //!< return true
                bool discard( const token & ); //!< return false
                bool newline( const token & ); //!< return false, increase iline


                //--------------------------------------------------------------
                //
                // rule: jump to another scanner
                //
                //--------------------------------------------------------------
                void jump(const string &id, pattern *p, const callback &on_jump);


                //______________________________________________________________
                //
                //! Get a new lexeme
                //______________________________________________________________
                /**
                 Longest First Come, First Saved algorithm.
                 When a rule is selected, its action is performed.
                 If the rule is a control rule, the token is discarded
                 and the action MUST return false.
                 */
                lexeme *get( source &src, ios::istream &fp, bool &ctrl);
                

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list       rules;
                l_list       cache;
                lexer       *lex;
                
                void check_label(const string &label);
                
            public:
                bool echo;
            };
        }
        
    }
}



#endif

