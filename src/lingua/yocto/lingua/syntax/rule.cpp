#include "yocto/lingua/syntax/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            rule:: ~rule() throw()
            {}


            rule:: rule(const string   &id,
                        const uint32_t  t)  :
            next(0),
            prev(0),
            label(id),
            flags(0),
            uuid(t),
            self(0),
            prv(0)
            {
            }

#if 0
            rule:: rule(const rule &other) :
            next(0),
            prev(0),
            label(other.label),
            flags(other.flags),
            uuid(other.uuid),
            self(0),
            prv(other.prv)
            {

            }
#endif



            void rule:: grow( xnode * &tree, xnode *leaf ) const throw()
            {
                assert(leaf);
                assert(0==leaf->parent);
                
                if(NULL==tree)
                {
                    tree = leaf;
                }
                else
                {
                    assert(false==tree->terminal);
                    tree->push_back(leaf);
                }
            }

            const string & rule::key() const throw()
            {
                return label;
            }

        }
    }
}
