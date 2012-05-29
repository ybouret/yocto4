#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        static const char new_object[] = "\\{";
        static const char new_array[]  = "\\[";
        static const char end_object[] = "\\}";
        static const char end_array[]  = "\\]";
        static const char NumberRX[]   = "[:digit:]+";
        
        class Parser :: Impl
        {
        public:
                       
            
            Impl() 
            {
               
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, ios::istream &fp )
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
            impl->call(value,in);
            return value;
        }
        
        
    }
    
}
