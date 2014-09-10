#include "yocto/lang/generator.hpp"

namespace yocto
{
    namespace lang
    {
        
        generator:: ~generator() throw()
        {
        }
        
        generator:: generator() :
        parser("generator","scan")
        {
            //__________________________________________________________________
            //
            // Generator Grammar
            //__________________________________________________________________
            
            
            //__________________________________________________________________
            //
            // final
            //__________________________________________________________________
            scanner.make("blank", "[:blank:]", discard);
            scanner.make("endl" , "[:endl:]",  newline);

            
        }
        
    }
    
}


