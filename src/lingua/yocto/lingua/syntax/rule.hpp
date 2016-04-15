#ifndef YOCTO_LINGUA_SYNTAX_RULE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_RULE_INCLUDED 1

#include "yocto/lingua/lexer.hpp"
#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace lingua
    {
        struct property
        {
            static const uint32_t standard  = 0x00; //!< full terminal
            static const uint32_t univocal  = 0x01; //!< single content terminal
            static const uint32_t jettison  = 0x02; //!< syntax only or temporary aggregate
            static const uint32_t noSingle  = 0x04; //!< aggregated is merged if 1 child
        };

        namespace syntax
        {


#define YOCTO_LINGUA_SYNTAX_RULE_IS(DERIVED) do { assert(0==self); DERIVED *addr = this; self = addr; } while(false)
#define YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS xnode * &tree, lexer &lxr, source &src

            //! a rule to accept the output of a lexer
            class rule : public object
            {
            public:
                static const char internal_char='#';
                typedef addr_node<const rule> meta_node;
                typedef addr_list<const rule> meta_list;

                rule          *next;
                rule          *prev;
                const string   label;
                uint32_t       flags;
                const uint32_t uuid;
                void          *self;
                meta_list      following;

                virtual ~rule() throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const = 0;
                virtual bool admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const = 0;
                virtual bool admit_empty() const throw() = 0;

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                void setup_following( meta_list &f ) const;
                void build_following();
                inline const string & key() const throw() { return label; }
                
            protected:
                explicit rule(const string &id,const uint32_t t);
                void     grow( xnode * &tree, xnode *leaf ) const throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

            typedef core::list_of_cpp<rule> r_list;
        }
    }
}

#endif

