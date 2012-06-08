#include "yocto/lang/compiler.hpp"

namespace yocto 
{
    namespace  lang 
    {
        
        compiler:: ~compiler() throw()
        {
            
        }
        
        compiler:: compiler() :
        parser( "lang.compiler.lexer", "compiler.grammar" )
        {
            //------------------------------------------------------------------
            //
            //------------------------------------------------------------------
            
        }
        
        
    }
    
}