#include "yocto/lang/compiler.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{

    namespace lang
    {
        
#define __CHECK(EXPR,ERROR) do { if( !(EXPR) ) throw exception("%s: " #ERROR,fn ); } while(false)
        
        grammar * compiler:: build( syntax::parse_node *ast) const
        {
            static const char fn[] = "lang.compiler.build(ast)";
            __CHECK(ast!=NULL,"NULL ast");
            __CHECK(! ast->terminal,"invalid ast");
            return NULL;
        }

    }

}
