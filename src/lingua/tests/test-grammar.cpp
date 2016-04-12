#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(grammar)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
    std::cerr << "sizeof(xlist)=" << sizeof(syntax::xlist) << std::endl;

    const string label = "term";
    syntax::terminal tt(label);
    token        data  = "hello";
    lexeme      *lxm   = new lexeme(label,1,data);

    auto_ptr<syntax::xnode> pTerm( syntax::xnode::create(tt,lxm) );

}
YOCTO_UNIT_TEST_DONE()
