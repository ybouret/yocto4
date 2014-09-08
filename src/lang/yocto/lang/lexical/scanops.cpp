#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/lang/pattern/compiler.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
                       
            
            bool scanner:: forward( const token &tkn )
            {
                return true;
            }
            
            
            
            bool scanner:: discard(const token &tkn )
            {
                //if(echo) std::cerr << tkn << std::endl;
                return false;
            }
            
            void scanner:: newline(const token &)
            {
                ++line;
            }
            
            bool scanner:: discard_newline(const token &t)
            {
                newline(t);
                return false;
            }
            
            bool scanner:: forward_newline(const token &t)
            {
                newline(t);
                return true;
            }
        }
    }
}
