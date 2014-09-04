#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;
#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(xnode)
{
    
    std::cerr << "sizeof(syntax::xnode)=" << sizeof(syntax::xnode) << std::endl;
    
    const string termID = "term";
    auto_ptr<syntax::xnode> pTerm( syntax::xnode::create(termID, new lexeme(termID,1)));
    *static_cast<token *>(pTerm->lxm()) = "hello";
    
    const string otherID = "other";
    auto_ptr<syntax::xnode> pOther( syntax::xnode::create(otherID) );
    pOther->add( pTerm.yield() );
    
    
}
YOCTO_UNIT_TEST_DONE()
