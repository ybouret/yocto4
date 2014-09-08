#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/lang/pattern/compiler.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
                       
            
            bool scanner:: emit( const token &tkn )
            {
                //if(echo) std::cerr << tkn << std::endl;
                return true;
            }
            
            
            
            bool scanner:: drop(const token &tkn )
            {
                //if(echo) std::cerr << tkn << std::endl;
                return false;
            }
            
            void scanner:: on_newline(const token &)
            {
                ++line;
            }
            
            bool scanner:: newline(const token &t)
            {
                on_newline(t);
                return false;
            }
            
            bool scanner:: newline_emit(const token &t)
            {
                on_newline(t);
                return true;
            }
        }
    }
}
