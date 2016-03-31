#ifndef YOCTO_LANG_SYNTAX_AGG_INCLUDED
#define YOCTO_LANG_SYNTAX_AGG_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/logic.hpp"
#include "yocto/code/fourcc.hpp"
namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            //! a following rules...
            class following_rule
            {
            public:
                following_rule *next;
                following_rule *prev;
                const rule     *self;
                const void     *addr;
                const uint32_t  uuid; //!< terminal or aggregate
                following_rule(const rule *r) throw();
                ~following_rule() throw();

                YOCTO_MAKE_OBJECT
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(following_rule);
            };


            //! base class for following rules
            typedef core::list_of_cpp<following_rule> _following_rules;

            //! possible following rules of an aggregate
            class following_rules : public _following_rules
            {
            public:
                explicit following_rules() throw();
                virtual ~following_rules() throw();

                void collect(const rule *);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(following_rules);
            };

            class aggregate : public logical
            {
            public:

                static const uint32_t UUID = YOCTO_FOURCC('[', '&', '&', ']');
                const property  modifier; //!< passed to xnode
                following_rules followers;   //!< possible next char

                virtual ~aggregate() throw();
                explicit aggregate(const string &id, const property ppty = standard);

                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;


                //! syntactic sugar
                aggregate & operator+=( rule &r );

                //! build followers
                void compile();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
            };
        }

    }

}

#endif
