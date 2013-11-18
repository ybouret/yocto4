#ifndef YOCTO_JSON_PARSER_INCLUDED
#define YOCTO_JSON_PARSER_INCLUDED 1

#include "yocto/json/json.hpp"
#include "yocto/lingua/source.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        
        class Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();
            
            Value & operator()( lingua::input & );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Parser);
            class Impl;
            Value value;
            Impl *impl;
        };
        
        
    }
    
}

#endif
