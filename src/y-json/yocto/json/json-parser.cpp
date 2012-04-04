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
            
            Impl() : lexer( "Waiting For First Object Or Array" ),
            iLine(1)
            {
                regex::sublexer &first = main();
                makeBlanks(first);
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
