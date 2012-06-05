#ifndef YOCTO_LANG_SYNTAX_JOKER_INCLUDED
#define YOCTO_LANG_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax
        {
            
            //! reference to a previous rule
            class joker : public rule 
            {
            public:
                virtual ~joker() throw();
                
            protected:
                explicit joker( const string &id, rule &r );
                rule &ref;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };
            
            class optional : public joker
            {
            public:
                explicit optional( const string &id, rule &r );
                virtual ~optional() throw();
                
                virtual bool  match( Y_SYNTAX_MATCH_ARGS );  
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };
        }
        
    }
}

#endif
