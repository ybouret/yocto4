#include "yocto/lingua/syntax/aggregate.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            aggregate:: ~aggregate() throw() {}
            
            aggregate:: aggregate( const string &id ) :
            composite(id)
            {
            }
            
            
            bool aggregate:: match(Y_SYNTAX_MATCH_ARGS)
            {
                const size_t n = items.size();
                if(n<=0)
                    throw exception("syntax.aggregate.%s is empty", label.c_str());
                xnode *node = xnode::create(label, 0, is_regular);
                auto_ptr<xnode> q(node);
                
                return true;
            }
            

        }

    }

}
