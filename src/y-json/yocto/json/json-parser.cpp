#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto 
{
    using namespace lang;
    
    namespace JSON
    {
        
        
        class Parser :: Impl : public object, public lexer, public grammar
        {
        public:
            
            
            lexical::scanner  &scan;
            lexical::scanner  &jstr;
            string             _str;
            
            Impl() : 
            lexer(   "JSON::Lexer" ),
            grammar( "JSON::Parser" ),
            scan( first() ),
            jstr( declare("JSON::String") ),
            _str()
            {
                
                //--------------------------------------------------------------
                //
                // declare JSON terminals
                //
                //--------------------------------------------------------------
                scan.make("LBRACK","\\[" );
                scan.make("RBRACK","\\]" );
                scan.make("LBRACE", "\\{");
                scan.make("RBRACE", "\\}");
                scan.make("COMMA",  ","  );
                scan.make("null",   "null" );
                scan.make("true",   "true" );
                scan.make("false",  "false" );
                scan.make("NUMBER", "[:digit:]+" );
                scan.make("COLUMN", ":");
                scan.call("JSON::String",  "\"", this, &Impl::OnEnterString);
                scan.make( "BLANKS", "[ \t]+", & scan.discard );
                scan.make( "ENDL", "[:endl:]", & scan.newline );
                
                jstr.back("\"", this, &Impl::OnLeaveString);
                jstr.make("1CHAR", ".", this, &Impl::OnChar);
                
                
                //--------------------------------------------------------------
                //
                // declare JSON Grammar
                //
                //--------------------------------------------------------------
                
                //-- main rule
                syntax::alternate & INSTANCE = alt("INSTANCE");
                
                //--------------------------------------------------------------
                // terminals
                //--------------------------------------------------------------
                syntax::terminal &LBRACK = term( "LBRACK", syntax::is_discardable );
                syntax::terminal &RBRACK = term( "RBRACK", syntax::is_discardable );
                
                //--------------------------------------------------------------
                // ARRAY
                //--------------------------------------------------------------
                syntax::alternate &ARRAYS = alt( "ARRAYS" );
                
                {
                    syntax::aggregate &EMPTY_ARRAY = agg( "EMPTY_ARRAY" );
                    EMPTY_ARRAY << LBRACK << RBRACK;
                    ARRAYS |= EMPTY_ARRAY;
                }
                
                INSTANCE |= ARRAYS;
                
                
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            void OnEnterString( const regex::token & )
            {
                _str.clear();
            }
            
            void OnLeaveString( const regex::token & )
            {
                unget( jstr, _str);
            }
            
            bool OnChar( const regex::token &t )
            {
                for( regex::t_char *ch = t.head; ch; ch=ch->next )
                    _str.append( ch->data );
                
                return false; // not a lexeme !
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
                while( (lx=next_lexeme(src)) != NULL )
                {
                    std::cerr << "line " << lx->line << "\t<" << lx->label << "> = '" << *lx << "'" << std::endl;
                    delete lx;
                }
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
