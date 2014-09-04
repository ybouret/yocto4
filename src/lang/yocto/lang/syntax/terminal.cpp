#include "yocto/lang/syntax/terminal.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            terminal:: ~terminal() throw()
            {
            }
            
            terminal:: terminal(const string &id) :
            rule(id)
            {
            }
            
            rule * terminal:: create(const string &id)
            {
                return new terminal(id);
            }
            
        }

    }

}
