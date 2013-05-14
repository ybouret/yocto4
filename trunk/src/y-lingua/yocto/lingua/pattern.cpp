#include "yocto/lingua/pattern.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/chars.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        pattern:: ~pattern() throw() {}
        
        
        pattern:: pattern(uint32_t t) throw()  :
        type(t),
        prev(0),
        next(0),
        data(0)
        {}
        
        void pattern:: reset() throw()
        {
            clear();
        }
        
        string  pattern:: make_name() const
        {
            string ans;
            ios::osstream fp(ans);
            save(fp);
            for(size_t i=0; i<ans.size();++i)
                ans[i] = make_visible(ans[i]);
            return ans;
        }

    }
    
}
