#include "yocto/lang/parser.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdlib>

#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"

using namespace yocto;
using namespace lang;


namespace
{
    
    class MyParser : public parser
    {
    public:
        MyParser() : parser("my parser", "main")
        {
            
            Terminal &LBRACK = jettison("LBRACK", '[');
            Terminal &RBRACK = jettison("RBRACK", ']');
            Terminal &INT    = terminal("INT", "[:digit:]+");
            Terminal &ID     = terminal("ID", "[:alpha:][:word:]+");
            Terminal &COMMA  = jettison("COMMA", ',');
            
            Alternate &ITEM = alt();
            ITEM << INT << ID << plug_term( new lexical::cstring("CSTRING",*this) );
            
            Aggregate &EXTRA_ITEM = merge();
            EXTRA_ITEM << COMMA << ITEM ;
            
            Aggregate &CONTENT = merge();
            CONTENT << ITEM << zero_or_more(EXTRA_ITEM);
            
            
            Aggregate &ZeroVector = agg("ZeroVector");
            ZeroVector << LBRACK << RBRACK;
            
            Aggregate &SomeVector = agg("SomeVector");
            SomeVector << LBRACK << CONTENT << RBRACK;
            
            Alternate &Vector = alt();
            Vector << ZeroVector << SomeVector;
            
            ITEM << Vector;
            
            set_root( zero_or_more(Vector,"Data") );
            
            // somme comment
            plug_meta( new lexical::comment("COMMENT", *this, "//") );
            plug_meta( new lexical::C_comment("C_COM", *this)       );
            
            // final
            scanner.make("BLANK", "[:blank:]", discard);
            scanner.make("ENDL",  "[:endl:]",  newline);
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
