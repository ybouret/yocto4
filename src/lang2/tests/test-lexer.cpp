#include "yocto/lang/lexer.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;

namespace{


    class Lexer : public lexer
    {
    public:
        virtual ~Lexer() throw() {}

        inline Lexer() : lexer("Lexer","root")
        {

            lexical::scanner &root = get_root();

            root.emit("ID","[_[:alpha:]][:word:]*");
            root.emit("INT", "[:digit:]+");

            root.call("COM", "//");
            root.call("COM2", "/\\*");

            lexical::scanner &com = declare("COM");
            com.back("[:endl:]", &com, &lexical::scanner::newline_cb);
            com.drop("CHAR",".");

            lexical::scanner &com2 = declare("COM2");
            com2.back("\\*/", &root, &lexical::scanner::discard_cb);
            com2.endl("ENDL");
            com2.drop("CHAR",".");

            lexical::plugin &bash_com = load<lexical::comment>("BashComment","#");
            bash_com.hook(root);

            lexical::plugin &xml_com = load<lexical::ccomment>("XmlComment","<!--","-->");
            xml_com.hook(root);

            lexical::plugin &cstring =load<lexical::cstring>("CSTRING");
            cstring.hook(root);

            root.drop("WS", "[:blank:]");
            root.endl("ENDL");
            root.emit("SNGL", ".");
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lexer);
    };

}

YOCTO_UNIT_TEST_IMPL(lexer)
{

    Lexer LX;

    ios::icstream fp(ios::cstdin);
    lang::source  src;

    lexeme *lx = 0;
    l_list  lexemes;

    while( NULL !=(lx=LX.get(src,fp) ) )
    {
        std::cerr << lx->label << "='" << *lx << "'" << std::endl;
        lexemes.push_back(lx);
    }

}
YOCTO_UNIT_TEST_DONE()

