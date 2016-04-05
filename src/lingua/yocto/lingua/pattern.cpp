#include "yocto/lingua/pattern.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"

namespace yocto
{
    namespace lingua
    {
        pattern:: ~pattern() throw()
        {
        }

        pattern:: pattern(const uint32_t t) throw() :
        next(0),
        prev(0),
        uuid(t)
        {
        }

        pattern:: pattern(const pattern &other) throw() :
        next(0),
        prev(0),
        uuid(other.uuid)
        {
        }

        void pattern:: save(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(uuid);
            __out(fp);
        }

        void pattern:: graphviz(const string &filename) const
        {
            ios::wcstream fp(filename);
            fp << "digraph G {\n";
            __viz(fp);
            fp << "}\n";
        }

        string pattern:: binary() const
        {
            string        ans;
            {
                ios::osstream fp(ans);
                save(fp);
            }
            return ans;
        }
        
    }
}
