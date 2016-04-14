#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


            static inline
            void grow_following( rule::meta_list &f, const rule *sub )
            {
                if(terminal::UUID==sub->uuid||aggregate::UUID==sub->uuid)
                {
                    f.append(sub);
                }
                else
                {
                    rule::meta_list sub_f;
                    sub->setup_following(sub_f);
                    f.merge_back(sub_f);
                }
            }

            void rule:: setup_following( meta_list &f ) const
            {
                assert(self);
                switch(uuid)
                {
                    case terminal::UUID:
                        break;

                    case optional::UUID:
                        grow_following(f, static_cast<const optional *>(self)->jk );
                        break;

                    case at_least::UUID:
                        grow_following(f, static_cast<const at_least *>(self)->jk );
                        break;

                    case alternate::UUID: {
                        const meta_list &m = static_cast<const alternate *>(self)->members;
                        for(const meta_node *node = m.head;node;node=node->next)
                        {
                            grow_following(f,node->addr);
                        }
                    } break;

                    case aggregate::UUID: {
                        const meta_list &m = static_cast<const alternate *>(self)->members;
                        for(const meta_node *node = m.head;node;node=node->next)
                        {
                            const rule *sub = node->addr;
                            grow_following(f,sub);
                            if(false==sub->admit_empty())
                            {
                                break;
                            }
                        }
                    } break;

                    default:
                        throw exception("unexpected UUID for rule '%s'", label.c_str());
                }

            }

            void rule::build_following()
            {
                following.clear();
                setup_following(following);
            }
            
            
        }
        
    }
    
}
