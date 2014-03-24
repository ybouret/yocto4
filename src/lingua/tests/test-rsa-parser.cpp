#include "yocto/utest/run.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lingua;


namespace
{
    
    class rsa_parser : public parser
    {
    public:
        explicit rsa_parser() :
        parser( "RSA", "main")
        {
            //__________________________________________________________________
            //
            // syntax element
            //__________________________________________________________________
            syntax::terminal &KEY  = terminal("KEY", "[:alpha:][[:word:]\\-]*:");
            syntax::terminal &INFO = jettison("INFO","\\([[:word:][:blank:]]*\\)");
            syntax::terminal &INT  = terminal("INT","[:digit:]+");
            syntax::terminal &HEX  = terminal("HEX","[:xdigit:][:xdigit:](:[:xdigit:][:xdigit:])*");
            //__________________________________________________________________
            //
            // other lexical elements
            //__________________________________________________________________
            Y_LEX_DISCARD(scanner, "BLANK", "[:blank:]");
            Y_LEX_NO_ENDL(scanner);
            
            
        }
        
        
        virtual ~rsa_parser() throw() {}
        
    private:
        YOCTO_DISABLE_ASSIGN(rsa_parser);
    };
}

YOCTO_UNIT_TEST_IMPL(rsa_parser)
{
    vfs &fs = local_fs::instance();
    if(fs.is_reg("tree.dot" )) fs.remove_file("tree.dot");
    if(fs.is_reg("tree.png"))  fs.remove_file("tree.png");

    
    rsa_parser Parser;
    source     Source;
    Source.attach_stdin();
    
#if 0
    auto_ptr<syntax::xnode> tree( Parser.run(Source) );
    if( tree.is_valid() )
    {
        std::cerr << "got tree..." << std::endl;
        tree->graphviz("tree.dot");
        int res=system("dot -Tpng -otree.png tree.dot");(void)res;
    }
    else
    {
        std::cerr << "empty tree!" << std::endl;
    }
    
#else 
    lexemes lxms;
    bool    fctl=false;
    while(true)
    {
        lexeme *lx = Parser.scanner.get(Source, fctl);
        if(lx)
        {
            lxms.push_back(lx);
            std::cerr << "[" << lx->label << "]=<" << *lx << ">" << std::endl;
        }
        if(fctl)
            throw exception("Unexpected Control Lexeme!");
        
        if( !lx )
            break;
    }
#endif
    
}
YOCTO_UNIT_TEST_DONE()
