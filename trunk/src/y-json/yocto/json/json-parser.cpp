#include "yocto/json/parser.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        
        class Parser:: Impl : public regex::lexer
        {
        public:
            typedef regex::lexical::action Action;
            unsigned iLine;
            
            inline void makeBlanks( regex::sublexer &lx )
            {
                const Action __drop( this, &Impl::Drop );
                const Action __endl( this, &Impl::Endl );
                const Action ___bad( this, &Impl::Bad  );
                lx.make( "[:endl:]",  __endl );
                lx.make( "[:blank:]", __drop );
                lx.make( regex::basic::any1::create(), ___bad );
            }
            
            Impl() : lexer( "JSON:: First Object Or Array" ),
            iLine(1)
            {
                static const char new_object[] = "\\{";
                static const char new_array[]  = "\\[";
                static const char end_object[] = "\\}";
                static const char end_array[]  = "\\]";
                
                regex::sublexer &makeObject = declare("JSON::Object parser");
                regex::sublexer &makeArray  = declare("JSON::Array parser");
                
                //--------------------------------------------------------------
                // first=main
                //--------------------------------------------------------------
                regex::sublexer &first = main();
                first.call( makeObject.name, new_object, this, & Impl:: NewObject );
                first.call( makeArray.name,  new_array,  this, & Impl:: NewArray  );
                makeBlanks(first);
                
                //--------------------------------------------------------------
                // makeObject
                //--------------------------------------------------------------
                makeObject.back( end_object, this, & Impl:: EndObject );
                makeBlanks( makeObject );
                
                //--------------------------------------------------------------
                // makeArray
                //--------------------------------------------------------------
                makeArray.back( end_array, this, & Impl:: EndArray );
                makeBlanks( makeArray );
                
                
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, ios::istream &fp )
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const Value null;
                value = null;
                iLine = 1;
                this->reset();
                
                //--------------------------------------------------------------
                // run
                //--------------------------------------------------------------
                regex::source src(fp);
                run(src);
            }
            
            //==================================================================
            // default
            //==================================================================
            void Drop( const regex::token & ) throw() {}
            
            void Endl( const regex::token & ) throw()
            {
                std::cerr << "[#" << iLine << "]" << std::endl;
                ++iLine;
            }
            
            void Bad( const regex::token &tkn )
            {
                assert( tkn.size == 1 );
                throw exception("JSON(unexpected char '%c') in %s", tkn.head->data, active_cstr() );
            }
            
            //==================================================================
            // Object
            //==================================================================
            void NewObject( const regex::token & )
            {
                std::cerr << "NewObject" << std::endl;
            }
            
            void EndObject( const regex::token & )
            {
                std::cerr << "EndObject" << std::endl;
            }
            
            
            //==================================================================
            // Array
            //==================================================================
            void NewArray( const regex::token & )
            {
                std::cerr << "NewArray" << std::endl;
            }
            
            void EndArray( const regex::token & )
            {
                std::cerr << "EndArray" << std::endl;
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
