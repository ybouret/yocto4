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
            syntax::terminal & LBRACK = jettison("LPAREN", "\\[");
            syntax::terminal & RBRACK = jettison("LBRACK", "\\]");
            syntax::terminal & ID     = terminal("ID", "[:alpha:][:word:]*");
            syntax::terminal & INT    = terminal("INT", "[:digit:]+");
            syntax::terminal & COMMA  = jettison("COMMA", ",");
            syntax::terminal & STOP   = jettison("STOP",  ";");
            
            lexical::scanner & COM1   = declare("COM1");
            scanner.call("COM1", "//", this, & MyParser:: EnterCom1);
            Y_LEX_DISCARD(COM1, "DOT", ".");
            COM1.back("[:endl:]", this, & MyParser:: LeaveCom1 );
            
            Y_LEX_DISCARD(scanner, "BLANK", "[:blank:]");
            Y_LEX_NO_ENDL(scanner);
            
            syntax::alternative & ITEM =  alt();
            ITEM |= ID;
            ITEM |= INT;
            
            syntax::aggregate & LIST = agg("LIST");
            LIST += LBRACK;
            LIST += ITEM;
            
            ITEM |= LIST;
            
            syntax::aggregate & OPT_ITEM = agg("OPT_ITEM", syntax::is_merging_all);
            OPT_ITEM += COMMA;
            OPT_ITEM += ITEM;
            
            syntax::repeating & EXTRA_ITEMS = rep("EXTRA_ITEMS", OPT_ITEM,0);
            LIST += EXTRA_ITEMS;
            LIST += RBRACK;
            
            
            syntax::aggregate & DECL = agg("DECL", syntax::is_merging_one);
            DECL += LIST;
            DECL += STOP;
            
            syntax::repeating & STAT = rep("STAT", DECL, 0);
            set_root( STAT );
            
        }
        
        virtual ~MyParser() throw()
        {
        }
        
        inline void EnterCom1( const token &)
        {
            std::cerr << "<Comment>" << std::endl;
        }
        
        inline void LeaveCom1( const token &)
        {
            std::cerr << "</Comment>" << std::endl;
            ++line;
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyParser);
    };
}

YOCTO_UNIT_TEST_IMPL(parser)
{
    vfs &fs = local_fs::instance();
    if(fs.is_reg("tree.dot" )) fs.remove_file("tree.dot");
    if(fs.is_reg("tree.png"))  fs.remove_file("tree.png");
    
    MyParser Parser;
    source   Source;
    Source.attach_stdin();
    auto_ptr<syntax::xnode> tree( Parser.run(Source) );
    
    if( tree.is_valid() )
    {
        tree->graphviz("tree.dot");
        int res=system("dot -Tpng -otree.png tree.dot");(void)res;
    }
    else
        std::cerr << "Empty Parse Tree" << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
