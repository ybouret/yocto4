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

            class aggregate : public logical
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '&', '&', ']');
                const property modifier; //!< passed to xnode
                virtual ~aggregate() throw();
                explicit aggregate(const string &id, const property ppty = standard);

                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;


                //! syntactic sugar
                aggregate & operator+=( rule &r );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
            };
        }

    }

}

#endif
