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
            
            Y_SYNTAX_MATCH_TYPE alternative:: match(Y_SYNTAX_MATCH_ARGS)
            {
                check(Tree);
                //std::cerr << "?ALT '" << label << "'" << std::endl;
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
                    xnode *altTree=0;
                    if( items[i]->match(Lexer, Source, altTree) )
                    {
                        //std::cerr << "+ALT '" << label << "'"  << std::endl;
                        if(altTree)
                        {
                            grow(Tree, altTree);
                        }
                        return true;
                    }
                }
                
                //std::cerr << "-ALT'" << label << "'" << std::endl;
                return false;
                
            }
            
        }
        
    }
    
}
