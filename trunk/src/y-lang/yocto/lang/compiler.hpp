#ifndef YOCTO_LANG_COMPILER_INCLUDED
#define YOCTO_LANG_COMPILER_INCLUDED 1


#include "yocto/lang/parser.hpp"


namespace yocto 
{
    
    namespace lang 
    {
        
        class compiler : public parser
        {
        public:
            explicit  compiler();
            virtual  ~compiler() throw();
            
            syntax::parse_node * operator()( ios::istream &fp );
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
        
        };
        
    }
    
}

#endif