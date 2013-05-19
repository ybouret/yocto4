#include "yocto/lingua/syntax/alternative.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            alternative:: ~alternative() throw()
            {
            }
            
            alternative:: alternative(const string &id) :
            composite(id)
            {
                
            }
            
            xnode * alternative:: match(Y_SYNTAX_MATCH_ARGS)
            {
                std::cerr << "?ALT '" << label << "'" << std::endl;
                //==============================================================
                // sanity check
                //==============================================================
                const size_t n = items.size();
                if(n<=0)
                    throw exception("syntax.alternative.%s is empty", label.c_str());
                
                //==============================================================
                // gather each rule
                //==============================================================
                for( size_t i=1; i <=n; ++i )
                {
                    xnode *node = items[i]->match(Lexer, Source);
                    if( node )
                    {
                        std::cerr << "+ALT '" << label << "'"  << std::endl;
                        return node;
                    }
                }
                
                std::cerr << "-AGG '" << label << "'" << std::endl;
                return 0;
                
            }
            
        }
        
    }
    
}
