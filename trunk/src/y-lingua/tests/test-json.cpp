#include "yocto/utest/run.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lingua;


namespace
{
    
    class MyJSON : public parser
    {
    public:
        explicit MyJSON() : parser( "JSON", "main" )
        {
            //! lexer
            syntax::terminal & LBRACE  = jettison("LBRACE", "\\{");
            syntax::terminal & RBRACE  = jettison("RBRACE", "\\}");
            syntax::terminal & LBRACK  = jettison("LBRACK", "\\[");
            syntax::terminal & RBRACK  = jettison("RBRACK", "\\]");
            syntax::terminal & STRING  = terminal("STRING", "[:cstring:]");
            syntax::terminal & JTRUE   = univocal("true", "true");
            syntax::terminal & JFALSE  = univocal("false", "false");
            syntax::terminal & JNULL   = univocal("null","null");
            syntax::terminal & JNUMBER = terminal("NUMBER","[:digit:]+");
            syntax::terminal & COLUMN  = jettison("COLUMN",":");
            syntax::terminal & COMMA   = jettison("COMMA", ",");
            
            Y_LEX_DISCARD(scanner, "BLANK", "[:blank:]");
            Y_LEX_NO_ENDL(scanner);
            
            
            //! grammar
            syntax:: aggregate &EMPTY_OBJ =  agg("EMPTY_OBJ");
            EMPTY_OBJ += LBRACE;
            EMPTY_OBJ += RBRACE;
            
            syntax::aggregate &EMPTY_ARR = agg("EMPTY_ARR");
            EMPTY_ARR += LBRACK;
            EMPTY_ARR += RBRACK;
            
           
            
            syntax::aggregate   &OBJECT = agg("OBJECT");
            syntax::aggregate   &ARRAY  = agg("ARRAY");
            syntax::alternative &VALUE  = alt("VALUE");
            VALUE |= STRING;
            VALUE |= JNUMBER;
            VALUE |= JTRUE;
            VALUE |= JFALSE;
            VALUE |= JNULL;
            VALUE |= OBJECT;
            VALUE |= EMPTY_OBJ;
            VALUE |= ARRAY;
            VALUE |= EMPTY_ARR;
            
            syntax::aggregate &PAIR= agg("PAIR");
            PAIR += STRING;
            PAIR += COLUMN;
            PAIR += VALUE;
            
            syntax::aggregate &EXTRA_PAIR = agg("EXTRA_PAIR", syntax::is_merging_all);
            EXTRA_PAIR += COMMA;
            EXTRA_PAIR += PAIR;
            
            OBJECT += LBRACE;
            OBJECT += PAIR;
            OBJECT += rep("EXTRA_PAIRS", EXTRA_PAIR, 0);
            OBJECT += RBRACE;
            
            syntax::aggregate &EXTRA_VALUE = agg("EXTRA_VALUE",syntax::is_merging_all);
            EXTRA_VALUE += COMMA;
            EXTRA_VALUE += VALUE;
            
            ARRAY += LBRACK;
            ARRAY += VALUE;
            ARRAY += rep("EXTRA_VALUES", EXTRA_VALUE,0);
            ARRAY += RBRACK;
            
            syntax::alternative &JSON = alt("JSON");
            JSON |= EMPTY_OBJ;
            JSON |= EMPTY_ARR;
            JSON |= OBJECT;
            JSON |= ARRAY;
            
            set_root(JSON);
            
        }
        
        virtual ~MyJSON() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyJSON);
        
    };
    
}

YOCTO_UNIT_TEST_IMPL(json)
{
    
    vfs &fs = local_fs::instance();
    if(fs.is_reg("tree.dot" )) fs.remove_file("tree.dot");
    if(fs.is_reg("tree.png"))  fs.remove_file("tree.png");

    
    MyJSON json;
    source src;
    src.attach_stdin();
    
    auto_ptr<syntax::xnode> root( json.run(src) );
    assert( root.is_valid() );
    root->graphviz("tree.dot");
    int res=system("dot -Tpng -otree.png tree.dot");(void)res;
    
}
YOCTO_UNIT_TEST_DONE()
