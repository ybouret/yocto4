#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto 
{
    using namespace lang;
    
    namespace JSON
    {
        static const char new_object[] = "\\{";
        static const char new_array[]  = "\\[";
        static const char end_object[] = "\\}";
        static const char end_array[]  = "\\]";
        static const char NumberRX[]   = "[:digit:]+";
        
        class Parser :: Impl : public lexer
        {
        public:
            enum status_type
            {
                WaitingForArrayOrObject
            };
            
            
            lexical::scanner  &scan;
            status_type        status;
            
            Impl() : lexer( "JSON::Parser" ),
            scan( first() ),
            status( WaitingForArrayOrObject )
            {
                
                scan.make( "BLANKS", "[ \t\v\f]+", & scan.discard );
                scan.make( "ENDL",   "[:endl:]",   & scan.newline );
                scan.make( "NEW_ARRAY", new_array, this, & Impl::NewArray ); 
                scan.make( "END_ARRAY", end_array, this, & Impl::NewArray ); 
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, ios::istream &fp )
            {
                value.nullify();
                reset();
                regex::source src(fp);
                
                lexeme *lx = NULL;
                while( 0 != (lx=next_lexeme(src)) )
                {
                    delete lx;
                }
                
            }
            
            //==================================================================
            // Array
            //==================================================================
            bool NewArray( const regex::token & )
            {
                std::cerr << "NewArray@line " << line << std::endl;
                return false;
            }
            
            bool EndArray( const regex::token & )
            {
                std::cerr << "EndArray@line" << line << std::endl;
                return false;
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
