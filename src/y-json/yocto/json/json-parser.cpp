#include "yocto/json/parser.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/rx/lexical/mod-cstring.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        static const char new_object[] = "\\{";
        static const char new_array[]  = "\\[";
        static const char end_object[] = "\\}";
        static const char end_array[]  = "\\]";
        
        class Parser:: Impl : public regex::lexer
        {
        public:
            typedef regex::lexical::action  Action;
            unsigned                        iLine;
            regex::lexical::mod_cstring    *modString;
            
            inline void makeBlanks( regex::sublexer &lx )
            {
                const Action __drop( this, &Impl::Drop );
                const Action __endl( this, &Impl::Endl );
                const Action ___bad( this, &Impl::Bad  );
                lx.make( "[:endl:]",  __endl );
                lx.make( "[:blank:]", __drop );
                lx.make( regex::basic::any1::create(), ___bad );
            }
            
            inline void makeValues( regex::sublexer &lex )
            {
                
                lex.call( "JSON::Object Init", new_object, this, & Impl:: NewObject );
                lex.call( "JSON::Array Init",  new_array,  this, & Impl:: NewArray  );
                lex.plug( modString->name );
                lex.make( "true",  this, & Impl::NewTrue  );
                lex.make( "false", this, & Impl::NewFalse );
                lex.make( "null",  this, & Impl::NewNull  );
            }
            
            Impl() : lexer( "JSON:: Wait for Value" ),
            iLine(1),
            modString( load<regex::lexical::mod_cstring,Impl>( this, & Impl::NewString, NULL ) )
            {
                
                regex::sublexer &ObjectInit      = declare("JSON::Object Init");
                regex::sublexer &ArrayInit       = declare("JSON::Array Init");
                regex::sublexer &ArrayNext       = declare("JSON::Array Next");
                
                //--------------------------------------------------------------
                // default: wait for value, should validate
                //--------------------------------------------------------------
                regex::sublexer &lex = main();
                lex.call( "JSON::Object Init", new_object, this, & Impl:: NewObject );
                lex.call( "JSON::Array Init",  new_array,  this, & Impl:: NewArray  );
                makeBlanks(lex);
                
                //--------------------------------------------------------------
                // makeObject
                //--------------------------------------------------------------
                ObjectInit.back( end_object, this, & Impl:: EndObject );
                makeBlanks( ObjectInit );
                
                //--------------------------------------------------------------
                //  ArrayInit
                //--------------------------------------------------------------
                ArrayInit.back( end_array, this, & Impl:: EndArray );
                
                makeBlanks( ArrayInit );
                
                //--------------------------------------------------------------
                //  ArrayNext
                //--------------------------------------------------------------
                makeBlanks(ArrayNext);
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
            
            //==================================================================
            // simple values
            //==================================================================
            void NewString( void *data )
            {
                assert(data!=NULL);
                const String &s = *(String *)data;
                std::cerr << "NewString: " << s << std::endl;
            }
            
            void NewTrue( const regex::token & )
            {
                std::cerr << "NewTrue" << std::endl;
            }
            
            void NewFalse( const regex::token & )
            {
                std::cerr << "NewTrue" << std::endl;
            }
            
            void NewNull( const regex::token & )
            {
                std::cerr << "NewNull" << std::endl;
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
