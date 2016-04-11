#include "yocto/lingua/lexer.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"
#include "yocto/lingua/lexical/plugin/block-comment.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"

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
            //root.emit("punct","[:punct:]");
            root.call("com1", "//", this, &my_lexer::ini_com1);
            root.call("com2", "/\\*", this, &my_lexer::ini_com2);
            root.endl("endl");

            lexical::scanner &com1 = declare("com1");
            com1.back("[:endl:]", this, & my_lexer::end_com1);
            com1.drop("any1","[\\x00-\\xff]");

            lexical::scanner &com2 = declare("com2");
            com2.endl("endl");
            com2.back("\\*/", this, & my_lexer::end_com2);
            com2.drop("any1","[\\x00-\\xff]");

            lexical::plugin &sh_com = load<lexical::end_of_line_comment>("shell comment","#");
            sh_com.hook(root);

            lexical::plugin &xml_com = load<lexical::block_comment>("xml comment","<!--","-->");
            xml_com.hook(root);

            lexical::plugin &cstr = load<lexical::cstring>("cstring");
            cstr.hook(root);

        }

        virtual ~my_lexer() throw()
        {
        }

        void ini_com1(const token &) throw()
        {
        }

        void end_com1(const token &) throw()
        {
            ++line;
        }

        void ini_com2(const token &)
        {
        }

        void end_com2(const token&)
        {

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
