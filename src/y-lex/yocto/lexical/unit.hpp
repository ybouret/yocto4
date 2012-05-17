#ifndef YOCTO_LEXICAL_UNIT_INCLUDED
#define YOCTO_LEXICAL_UNIT_INCLUDED 1

#include "yocto/rx/token.hpp"


namespace yocto 
{
    namespace lexical 
    {
        
        class unit : public regex::token
        {
        public:
            const string & type;
            const size_t   line;
            unit *         next;
            unit *         prev;
            explicit unit( const string &t, size_t l ) throw();
            virtual ~unit() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(unit);
        };
        
    }
}

#endif
