#include "yocto/lang/parser.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdlib>

#include "yocto/lang/lexical/plugin/cstring.hpp"

using namespace yocto;
using namespace lang;


namespace
{
    
    class JSON_Parser : public parser
    {
    public:
        JSON_Parser() : parser("JSON","Value")
        {
            // declare terminal
            Terminal &LBRACK = jettison("LBRACK", '[');
            Terminal &RBRACK = jettison("RBRACK", ']');
            Terminal &COMMA  = jettison("COMMA",  ',');
            Terminal &COLUMN = jettison("COLUMN", ':');
            Terminal &LBRACE = jettison("LBRACE", '{');
            Terminal &RBRACE = jettison("RBRACE", '}');
            Rule     &STRING = plug_term(new lexical::cstring("string",*this));
            
            // initialize value
            Alternate &value = alt();
            value |= STRING;
            value |= univocal("null",  "null");
            value |= univocal("false", "false");
            value |= univocal("true",  "true");
            const char number_expr[] = "-?(0|([1-9][:digit:]*))(\\.[:digit:]*([eE]-?[:digit:]+)?)?";
            //const char number_expr[] =  "[:digit:]+";
            pattern   *number_code   = compile(number_expr,NULL);
            value |= terminal("number",number_code);
            //number_code->graphviz("number.dot");
            //system("dot -Tpng -o number.png number.dot");
            
            
            // arrays
            Alternate &arrays = alt();
            {
                {
                    Aggregate &empty_array = assemble("empty_array");
                    empty_array << LBRACK << RBRACK;
                    arrays |= empty_array;
                }
                
                {
                    Aggregate &array       = assemble("array");
                    array << LBRACK << value;
                    {
                        Aggregate &extra_value  = merge();
                        extra_value << COMMA << value;
                        array << zero_or_more(extra_value);
                    }
                    
                    array << RBRACK;
                    
                    arrays |= array;
                }
            }
            value |= arrays;
            
            // object
            Alternate &objects = alt();
            {
                Aggregate &empty_object = assemble("empty_object",LBRACE,RBRACE);
                objects |= empty_object;
            }
            
            {
                Aggregate &heavy_object = assemble("object");
                heavy_object << LBRACE;
                {
                    Aggregate &Pair      = assemble("pair",STRING,COLUMN,value);
                    Aggregate &ExtraPair = merge(COMMA,Pair);
                    heavy_object << Pair << zero_or_more(ExtraPair);
                }
                heavy_object << RBRACE;
                objects |= heavy_object;
            }
            value |= objects;
            
            set_root( choose(objects,arrays) );
            
            // final
            scanner.make("BLANK", "[:blank:]", discard);
            scanner.make("ENDL",  "[:endl:]",  newline);
        }
        
        virtual ~JSON_Parser() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };
    
}

YOCTO_UNIT_TEST_IMPL(json)
{
    vfs &fs = local_fs::instance();
    
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");
    
    JSON_Parser prs;
    prs.show_rules();
    ios::icstream fp( ios::cstdin );
    
    if(argc>1 && 0 == strcmp("lex",argv[1]) )
    {
        while(true)
        {
            source src;
            lexeme *lx = prs.get( src, fp );
            if(!lx)
                break;
            
            auto_ptr<lexeme> LX(lx);
            std::cerr << lx->label << " : '" << *lx << "'" << std::endl;
        }
    }
    else
    {
        syntax::x_ptr Tree( prs.run(fp) );
        
        if(Tree.is_valid())
        {
            std::cerr << "rendering tree..." << std::endl;
            Tree->graphviz("xnode.dot");
            system("dot -Tpng -o xnode.png xnode.dot");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
