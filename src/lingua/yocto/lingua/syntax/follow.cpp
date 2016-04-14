#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/lingua/syntax/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void rule::build_following()
            {
                following.clear();
                switch(uuid)
                {
                    case terminal::UUID:   // nothing follow!
                        break;

                    case optional::UUID:{  //
                        rule *sub = (rule*)(static_cast<optional *>(self)->jk);
                        sub->build_following();
                        following.swap_with(sub->following);
                    } break;

                    case at_least::UUID: {
                        rule *sub = (rule*)(static_cast<at_least *>(self)->jk);
                        sub->build_following();
                        following.swap_with(sub->following);
                    } break;


                    case alternate::UUID: {
                        meta_list &m = static_cast<alternate *>(self)->members;
                        for(meta_node *node=m.head;node;node=node->next)
                        {

                        }
                    } break;
                        
                    default:
                        break;
                }
            }
            
            
        }
        
    }
    
}
