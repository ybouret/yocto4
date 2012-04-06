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
        static const char NumberRX[]   = "[:digit:]+";
        
        class Parser:: Impl : public regex::lexer
        {
        public:
            typedef regex::lexical::mod_cstring StringPlugin;
            typedef regex::lexical::action      Action;
            unsigned                            iLine;
            const string                        ArrayInitID;
            const string                        ArrayCommaID;
            StringPlugin                       *ArrayStringPlugin;
            
            inline void makeBlanks( regex::sublexer &lx )
            {
                const Action __drop( this, &Impl::Drop );
                const Action __endl( this, &Impl::Endl );
                const Action ___bad( this, &Impl::Bad  );
                lx.make( "[:endl:]",  __endl );
                lx.make( "[:blank:]", __drop );
                lx.make( regex::basic::any1::create(), ___bad );
            }
            
            inline void makeArray( regex::sublexer &lx )
            {
                //lx.jump( ArrayCommaID, new_array, this, & Impl::NewArray ); 
                lx.call( ArrayInitID,  new_array,  this, & Impl:: NewArray  );
                lx.jump( ArrayCommaID, "true",    this, & Impl::NewTrue  );
                lx.jump( ArrayCommaID, "false",   this, & Impl::NewFalse );
                lx.jump( ArrayCommaID, "null",    this, & Impl::NewNull  );
                lx.jump( ArrayCommaID, NumberRX,  this, & Impl::NewNumber );
            }
            
            
            Impl() : lexer( "JSON:: Wait for Value" ),
            iLine(1),
            ArrayInitID("JSON::Array Init"),
            ArrayCommaID( "JSON::Array Comma" ),
            ArrayStringPlugin( load<StringPlugin,Impl>( this, & Impl::NewString, NULL ) )
            {
                const string    ArrayNextID("JSON::Array Next");
                regex::sublexer &ObjectInit      = declare("JSON::Object Init");
                regex::sublexer &ArrayInit       = declare( ArrayInitID  );
                regex::sublexer &ArrayComma      = declare( ArrayCommaID );
                regex::sublexer &ArrayNext       = declare( ArrayNextID  );
                const Action __drop( this, &Impl::Drop );
                
                //--------------------------------------------------------------
                // default: wait for value, should validate
                //--------------------------------------------------------------
                regex::sublexer &lex = main();
                //lex.call( "JSON::Object Init", new_object, this, & Impl:: NewObject );
                lex.call( ArrayInitID,  new_array,  this, & Impl:: NewArray  );
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
                makeArray(  ArrayInit );
                makeBlanks( ArrayInit );
                
                //--------------------------------------------------------------
                //  ArrayComma
                //--------------------------------------------------------------
                ArrayComma.back( end_array, this, & Impl:: EndArray );
                ArrayComma.jump( ArrayNextID, ",",  __drop);
                makeBlanks(ArrayComma);
                
                //--------------------------------------------------------------
                //  ArrayNext
                //--------------------------------------------------------------
                makeArray(  ArrayNext );
                makeBlanks( ArrayNext ) ;
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
            
            void NewNumber( const regex::token &tkn )
            {
                std::cerr << "NewNumber: " << tkn << std::endl;
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
