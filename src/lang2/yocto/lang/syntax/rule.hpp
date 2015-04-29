#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

#define Y_LANG_SYNTAX_RULE_ACCEPT_ARGS xnode * &tree, lexer &lxr, source &src, ios::istream &fp

            class rule : public object
            {
            public:
                rule          *next;
                rule          *prev;
                const string   label;
                const uint32_t uuid;

                virtual ~rule() throw();


                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) const = 0;
                virtual void viz( ios::ostream &fp ) const = 0;
                virtual void lnk( ios::ostream &fp ) const = 0;

            protected:
                explicit rule(const string &id, uint32_t uu);
                
                void grow( xnode * &tree, xnode *leaf ) const throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

            typedef core::list_of_cpp<rule> r_list;
            
        }
    }
}

#endif
