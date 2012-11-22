#ifndef YOCTO_CSV_PARSER_INCLUDED
#define YOCTO_CSV_PARSER_INCLUDED 1

#include "yocto/csv/csv.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
    
    namespace CSV
    {
     
        class Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();
            
            void operator()(Lines &lines, ios::istream & );
            
        private:
            class Impl;
            Impl *impl;
            YOCTO_DISABLE_COPY_AND_ASSIGN(Parser);
        };
        
    }
    
}

#endif

