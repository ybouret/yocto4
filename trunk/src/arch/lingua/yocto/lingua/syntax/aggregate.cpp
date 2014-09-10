#include "yocto/lingua/syntax/aggregate.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            aggregate:: ~aggregate() throw() {}
            
            aggregate:: aggregate(const string &id ,
                                  node_property ppty) :
            composite(id),
            behavior(ppty)
            {
            }
            
            Y_SYNTAX_MATCH_TYPE aggregate:: match(Y_SYNTAX_MATCH_ARGS)
            {
                check(Tree);
                //std::cerr << "?AGG '" << label << "'" << std::endl;
                //==============================================================
                // sanity check
                //==============================================================
                const size_t n = items.size();
                if(n<=0)
                    throw exception("syntax.aggregate.%s is empty", label.c_str());
                
                
                //==============================================================
                // Create the SubTree
                //==============================================================
                xnode          *aggTree = xnode::create(label, 0, behavior);
                auto_ptr<xnode> protection(aggTree);
                
                
                //==============================================================
                // gather each rule
                //==============================================================
                for( size_t i=1; i <=n; ++i )
                {
                    if( !items[i]->match(Lexer, Source, aggTree) )
                    {
                        //--------------------------------------------------
                        // no throw, delete aggTree
                        //--------------------------------------------------
                        protection.forget();
                        xnode::restore(Lexer, aggTree);
                        //std::cerr << "-AGG '" << label << "'" << std::endl;
                        return false;
                    }
                }
                
                //==============================================================
                // grow the tree
                //==============================================================
                //std::cerr << "+AGG '" << label << "'" << std::endl;
                protection.forget();
                grow(Tree,aggTree);
                return true;
            }
            
        }
        
    }
    
}
