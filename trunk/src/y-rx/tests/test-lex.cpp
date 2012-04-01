#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/rx/lexical/mod-ccomment.hpp"
#include "yocto/rx/lexical/mod-cstring.hpp"

using namespace yocto;


namespace 
{
    
    class MyLexer : public regex::lexer
    {
    public:
        typedef regex::lexical::action   Action;
        typedef regex::lexical::callback Callback;
        
        size_t                        iline;
        regex::lexical::mod_ccomment *plugCom;
        regex::lexical::mod_cstring  *plugStr;
        
        MyLexer() : 
        regex::lexer("main"),
        iline(1),
        plugCom( NULL ),
        plugStr( NULL )
        {
            regex::sublexer &lex = main();
            const Callback  cb( this, & MyLexer::on_ccomment );
            const Callback  cb2( this, & MyLexer::on_cstring );
            load( plugCom = new regex::lexical::mod_ccomment(cb) );
            load( plugStr = new regex::lexical::mod_cstring(cb2) );
            
            const Action   __show( this, & MyLexer::show );
            const Action   __endl( this, & MyLexer::endl );
            const Action   __drop( this, & MyLexer::discard);
            const Action   __comment( this, & MyLexer::ini_comment );
            
            lex.make( "[:digit:]+",  __show );
            lex.make( "[:digit:]+f", __show );
            lex.make( "[:endl:]",    __endl );
            lex.call( "comment", "//", __comment );
            lex.plug( plugCom->name );
            lex.plug( plugStr->name );
            lex.make( ".", this, & MyLexer::discard);
            
            regex::sublexer &com = declare( "comment" );
            
            com.back( "[:endl:]", __endl );
            com.make( ".",        __drop );
                      
            
            plugCom->join("[:endl:]", __endl );
        }
        
        
        void on_ccomment( void *data )
        {
            assert(data);
            std::cerr << "Processing " << plugCom->name << std::endl;
            const string &s = *(string *)data;
            std::cerr << "[" << s << "]" << std::endl;
        }
        
        void on_cstring( void *data )
        {
            assert( data );
            std::cerr << "Processing " << plugStr->name << std::endl;
            const string &s = *(string *)data;
            std::cerr << "[" << s << "]" << std::endl;
        }
        
               
        void show( const regex::token &p ) 
        {
            std::cerr << "<" << p << ">" << std::endl;
        }
        
        void discard(  const regex::token &p ) throw(){}
        
        void endl( const regex::token &p )
        {
            std::cerr << "[#" << iline++ << "]" << std::endl;
        }
        
        void ini_comment( const regex::token & )
        {
            std::cerr << "[COMMENT]" << std::endl;
        }
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
    
}

YOCTO_UNIT_TEST_IMPL(lex)
{
    
    MyLexer Lex;
    Lex.reset();
    
        
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    Lex.run(src);
   
    
}
YOCTO_UNIT_TEST_DONE()
