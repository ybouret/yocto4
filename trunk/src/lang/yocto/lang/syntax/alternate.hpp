#ifndef YOCTO_LANG_SYNTAX_ALT_INCLUDED
#define YOCTO_LANG_SYNTAX_ALT_INCLUDED 1

#include "yocto/lang/syntax/compound.hpp"

namespace yocto 
{
    namespace lang
    {
        namespace syntax 
        {
            
            class alternate : public compound
            {
            public:
                explicit alternate( const string &id );
                virtual ~alternate() throw();
                
                virtual bool  match( Y_SYNTAX_MATCH_ARGS );
                
                alternate & operator|=( rule &r );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(alternate);
            };
        }
    }
}

#endif
