#include "yocto/lang/parser.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;


namespace
{
    
    class MyParser : public parser
    {
    public:
        MyParser() : parser("my parser", "main")
        {
            //
            syntax::alternate &VectorOrList = alt();
            
            syntax::terminal &INT    = terminal("INT", "[:digit:]+");
            syntax::terminal &COMMA  = jettison("COMMA", ',');
            syntax::terminal &LBRACK = jettison("LBRACK",'[');
            syntax::terminal &RBRACK = jettison("RBRACK",']');
            
            syntax::alternate &Content = alt();
            Content << INT;
            
            syntax::aggregate &EmptyVec = agg("EmptyVec");
            EmptyVec << LBRACK << RBRACK;
            
            syntax::aggregate &Vec = agg("Vec");
            Vec << LBRACK << Content;
            
            Vec << RBRACK;
            
            syntax::alternate &SomeVector = alt();
            SomeVector << EmptyVec;
            
            
            VectorOrList << SomeVector;
            
            set_root( zero_or_more("items", VectorOrList) );
            
            // final
            scanner.make("BLANK", "[:blank:]",discard);
            scanner.make("ENDL",  "[:endl:]", newline);
        }
        
        virtual ~MyParser() throw()
        {
        }
        
    };
    
}

#include "yocto/fs/local-fs.hpp"

YOCTO_UNIT_TEST_IMPL(parser)
{
    vfs &fs = local_fs::instance();
    
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");
    
    MyParser prs;
    prs.show_rules();
    
    ios::icstream fp( ios::cstdin );
    syntax::x_ptr Tree( prs.run(fp) );
    
    if(Tree.is_valid())
    {
        std::cerr << "rendering tree..." << std::endl;
        Tree->graphviz("xnode.dot");
        system("dot -Tpng -o xnode.png xnode.dot");
    }
    
}
YOCTO_UNIT_TEST_DONE()
