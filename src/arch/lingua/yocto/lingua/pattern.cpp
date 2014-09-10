#include "yocto/lingua/pattern.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/chars.hpp"

#include <cstring>

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
        
        string  pattern:: hr_name() const
        {
            string ans;
            ios::osstream fp(ans);
            save(fp);
            for(size_t i=0; i<ans.size();++i)
                ans[i] = make_visible(ans[i]);
            return ans;
        }
        
        string pattern:: content() const
        {
            string ans;
            ios::osstream fp(ans);
            save(fp);
            return ans;
        }

        
        bool operator==( const pattern &lhs, const pattern &rhs )
        {
            const string L = lhs.content();
            const string R = rhs.content();
            return L == R;
        }
        
        bool operator!=( const pattern &lhs, const pattern &rhs )
        {
            const string L = lhs.content();
            const string R = rhs.content();
            return L != R;
        }
        
        void pattern:: optimize() throw()
        {
            // default is do nothing
        }
        
        
        void pattern:: outviz( char c, ios::ostream &fp)
        {
            if(c<32 || c >= 127 || c == '"' || c == '\\' )
            {
                fp("0x%02x", unsigned(c) );
            }
            else
                fp("%c",c);
        }
        
        void pattern:: graphviz( const string &filename ) const
        {
            ios::ocstream fp(filename,false);
            fp << "digraph G{\n";
            viz(fp);
            fp << "}\n";
        }
        
        void pattern:: graphviz( const char   *fn ) const
        {
            const string filename(fn);
            graphviz(filename);
        }

        const char *pattern:: fourcc() const throw()
        {
            static char cc[8];
            memset(cc,0,sizeof(cc));
            cc[0] = char((type>>24) & 0xff);
            cc[1] = char((type>>16) & 0xff);
            cc[2] = char((type>>8 ) & 0xff);
            cc[3] = char((type)     & 0xff);
            return cc;
        }
        
    }
    
}
