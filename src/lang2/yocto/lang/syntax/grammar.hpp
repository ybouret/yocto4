#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {

        namespace syntax
        {
            class grammar : public object
            {
            public:
                explicit grammar(const string &id);
                virtual ~grammar() throw();

                const string name;

                //! accepting terminal
                rule &term(const string &label);

                //! set root from a previously declared rule
                void set_root( rule &r ) throw();

                //! try to accept first rule
                xnode *accept( lexer &lxr, source &src, ios::istream &fp);


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
                r_list rules;
                void check_label(const string &label) const;

            };
        }
        
    }
    
}

#endif

