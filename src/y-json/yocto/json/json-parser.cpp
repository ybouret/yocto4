#include "yocto/json/parser.hpp"
#include "yocto/rx/lexer.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        
        class Parser:: Impl : public regex::lexer
        {
        public:
            Impl() : lexer( "WaitForObjectOrArray" )
            {
                
                
                
            }
            
            virtual ~Impl() throw()
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Impl);
        };
        
        
        Parser:: ~Parser() throw() 
        {
            delete impl;
        }
        
        
        Parser:: Parser() :
        value(),
        impl( new Impl() )
        {
        }
        
        Value & Parser:: operator()( ios::istream &in )
        {
            const Value null;
            value = null;
            return value;
        }
        
        
    }
    
}
