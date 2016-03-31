#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

#define Y_LANG_SYNTAX_RULE_ACCEPT_ARGS  xnode * &tree, lexer &lxr, source &src, ios::istream &fp

            //! a syntax rule
            class rule : public object
            {
            public:
                typedef addr_node<rule> anode_t;
                typedef addr_list<rule> alist_t;
                
                rule             *next;
                rule             *prev;
                const string      label;
                const uint32_t    uuid;
                

                virtual ~rule() throw();
                
                //! main accept routine
                /**
                 upon success, the tree is grown
                 upon failure, the tree must be left unchanged
                 */
                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) const = 0;

                //! write name+label
                virtual void viz( ios::ostream &fp ) const = 0;

                //! write links
                virtual void lnk( ios::ostream &fp ) const = 0;

                //! for smart pointer
                const string & key() const throw();

                //! internal data to read specific stuff
                virtual void *content() const throw() = 0;

                void encode_label( ios::ostream &fp ) const;
                
                
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
