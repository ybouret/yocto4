#ifndef YOCTO_LANG_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LANG_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/lang/regexp.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            class scanner : public counted_object
            {
            public:
                virtual ~scanner() throw();
                explicit scanner(const string &id, int &ir);

                const string name;  //!< identifier
                int         &iline; //!< line index reference

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


                bool forward( const token & ); //!< return true
                bool discard( const token & ); //!< return false
                bool newline( const token & ); //!< return false, increase iline
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list       rules;
                l_list       cache;
                void check_label(const string &label);
                
            public:
                bool echo;
            };
        }
        
    }
}



#endif

