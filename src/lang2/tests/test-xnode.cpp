#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"

#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lang;

namespace{


    class Lexer : public lexer
    {
    public:
        virtual ~Lexer() throw() {}

        inline Lexer() : lexer("xLexer","root")
        {

            lexical::scanner &root = get_root();

            root.emit("ID",  "[_[:alpha:]][:word:]*");
            root.emit("INT", "[:digit:]+");

            lexical::plugin &cstring =load<lexical::cstring>("STR");
            cstring.hook(root);

            lexical::plugin &com     = load<lexical::comment>("COM","//");
            com.hook(root);

            root.drop("WS", "[:blank:]");
            root.endl("ENDL");
            root.emit("SNGL", ".");

        }



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lexer);
    };

}

YOCTO_UNIT_TEST_IMPL(xnode)
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
    
    std::cerr << "<Lexemes>" << std::endl;
    for(const lexeme *lx=lexemes.head;lx;lx=lx->next)
    {
        std::cerr << "\t" << lx->label << " : " << *lx << std::endl;
    }
    std::cerr << "</Lexemes>" << std::endl;

    auto_ptr<syntax::xnode> root( syntax::xnode::leaf() );
    while(lexemes.size)
    {
        root->append( syntax::xnode::term(lexemes.pop_front()) );
    }
    
}
YOCTO_UNIT_TEST_DONE()
