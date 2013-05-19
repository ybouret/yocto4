#include "yocto/lingua/syntax/aggregate.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            aggregate:: ~aggregate() throw() {}
            
            aggregate:: aggregate( const string &id , node_property ppty) :
            composite(id),
            behavior(ppty)
            {
            }
            
            xnode *aggregate:: match(Y_SYNTAX_MATCH_ARGS)
            {
                std::cerr << "?AGG '" << label << "'" << std::endl;
                //==============================================================
                // sanity check
                //==============================================================
                const size_t n = items.size();
                if(n<=0)
                    throw exception("syntax.aggregate.%s is empty", label.c_str());
                xnode          *parent = xnode::create(label, 0, behavior);
                auto_ptr<xnode> node( parent );
                
                //==============================================================
                // gather each rule
                //==============================================================
                for( size_t i=1; i <=n; ++i )
                {
                    xnode *sub = items[i]->match(Lexer, Source);
                    if( !sub )
                    {
                        xnode::restore(Lexer, node.yield() );
                        std::cerr << "-AGG '" << label << "'" << std::endl;
                        return NULL;
                    }
                    node->children().push_back(sub);
                    sub->parent = parent;
                }
                
                std::cerr << "+AGG '" << label << "'" << std::endl;
                return node.yield();
            }

        }

    }

}
