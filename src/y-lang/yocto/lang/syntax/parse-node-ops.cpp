#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            void parse_node:: compress() throw()
            {
                if( terminal == 0 )
                {
                    std::cerr << "compress " << label << std::endl;
                    child_list &chl = children();
                    child_list  tmp;
                    while( chl.size > 0 )
                    {
                        parse_node *node = chl.pop_front();
                        if( node->flags & discardable )
                        {
                            delete node;
                        }
                        else 
                        {
                            node->compress();
                            tmp.push_back(node);
                        }
                    }
                    chl.swap_with(tmp);
                }
            }
            
        }
        
    }
    
}
