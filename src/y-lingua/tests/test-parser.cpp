#include "yocto/utest/run.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lingua;


namespace
{
    
    class MyParser : public parser
    {
    public:
        explicit MyParser() : parser( "Lists", "main")
        {
            syntax::terminal & LBRACK = terminal("LPAREN", "\\[");
            syntax::terminal & RBRACK = terminal("LBRACK", "\\]");
            syntax::terminal & ID     = terminal("ID", "[:alpha:][:word:]*");
            syntax::terminal & INT    = terminal("INT", "[:digit:]+");
            syntax::alternative & ITEM =  alt("ITEM");
            ITEM |= ID;
            ITEM |= INT;
            
            set_root( opt(ITEM) );
        }

        virtual ~MyParser() throw()
        {
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyParser);
    };
}

YOCTO_UNIT_TEST_IMPL(parser)
{
    MyParser Parser;
    source   Source;
    Source.attach_stdin();
    
    if( Parser(Source) )
    {
        if( Parser.tree.is_valid() )
        {
            Parser.tree->graphviz("tree.dot");
            system("dot -Tpng -otree.png tree.dot");
        }
        else
            std::cerr << "Empty Parse Tree" << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
