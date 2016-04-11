#include "yocto/lingua/lexer.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lingua;

namespace
{
    class my_lexer : public lexer
    {
    public:
        my_lexer() : lexer("my_lexer","main")
        {
            root.emit("word","[:word:]+");
            root.emit("int","[:digit:]+");
            root.drop("blank","[:blank:]+");
            root.emit("punct","[:punct:]");
            root.call("com1", "//", this, &my_lexer::ini_com1);
            root.endl("endl");

            lexical::scanner &com1 = declare("com1");
            com1.drop("dot",".");
            com1.back("[:endl:]", this, & my_lexer::end_com1);

        }

        virtual ~my_lexer() throw()
        {
        }

        void ini_com1(const token &) throw()
        {
            std::cerr << "<C++ comment>" << std::endl;
        }

        void end_com1(const token &) throw()
        {
            std::cerr << "<C++ comment>" << std::endl;
            ++line;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(my_lexer);
    };
}

YOCTO_UNIT_TEST_IMPL(lexer)
{

    
    my_lexer      lxr;
    ios::icstream fp( ios::cstdin );
    source        src(fp);
    lexeme       *lx = 0;
    l_list        lexemes;

    while( 0 != ( lx=lxr.get(src) ) )
    {
        std::cerr << lx->line << ": '" << lx->label << "' => '" << *lx << "'" << std::endl;
        lexemes.push_back(lx);
    }
    std::cerr << "#lexemes=" << lexemes.size << std::endl;


}
YOCTO_UNIT_TEST_DONE()
