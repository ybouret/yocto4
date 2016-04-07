#include "yocto/lingua/pattern.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/string/base64.hpp"

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
        uuid(t),
        self(0)
        {
        }

        pattern:: pattern(const pattern &other) throw() :
        next(0),
        prev(0),
        uuid(other.uuid),
	self(0)
        {
        }

        void pattern:: save(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(uuid);
            __sav(fp);
        }

        void pattern:: graphviz(const string &filename) const
        {
            ios::wcstream fp(filename);
            fp << "digraph G {\n";
            __viz(fp);
            fp << "}\n";
        }

        string pattern:: bin() const
        {
            string        ans;
            {
                ios::osstream fp(ans);
                save(fp);
            }
            return ans;
        }

        string pattern:: b64() const
        {
            static base64 &Base64 = base64::instance();
            string s = bin();
            return Base64.Encode(s);
        }
        
    }
}
