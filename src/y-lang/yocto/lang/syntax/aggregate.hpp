#ifndef YOCTO_LANG_SYNTAX_AGG_INCLUDED
#define YOCTO_LANG_SYNTAX_AGG_INCLUDED 1

#include "yocto/lang/syntax/compound.hpp"

namespace yocto 
{
    namespace lang
    {
        namespace syntax 
        {
            
            class aggregate : public compound
            {
            public:
                explicit aggregate( const string &id );
                virtual ~aggregate() throw();
                
                virtual bool  match( Y_SYNTAX_MATCH_ARGS );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
            };
        }
    }
}

#endif
