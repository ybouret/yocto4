#include "yocto/lang/pattern.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/chars.hpp"

namespace yocto
{
    namespace lang
    {
        pattern:: ~pattern() throw()
        {
            
        }
        
        pattern:: pattern( const uint32_t user_type ) throw() :
        token(),
        next(0),
        prev(0),
        type(user_type),
        self(0)
        {
        }
        
        void pattern:: reset() throw() { clear(); }
     
        
        void pattern:: outviz( const char c, ios::ostream &fp )
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
            this->viz(fp);
            fp << "}\n";
        }
        
        void pattern:: graphviz( const char   *fn ) const
        {
            const string filename(fn);
            graphviz(filename);
        }

        void pattern:: refactor() throw()
        {
            
        }
        
        const char *pattern:: fourcc_of(uint32_t tag)  throw()
        {
            static char cc[8];
            memset(cc,0,sizeof(cc));
            cc[0] = make_visible(char((tag>>24) & 0xff));
            cc[1] = make_visible(char((tag>>16) & 0xff));
            cc[2] = make_visible(char((tag>>8 ) & 0xff));
            cc[3] = make_visible(char((tag)     & 0xff));
            return cc;
        }
        
        const char *pattern:: fourcc() const throw()
        {
            return fourcc_of(type);
        }

        string pattern:: to_binary() const
        {
            string        ans;
            ios::osstream fp(ans);
            save(fp);
            fp.flush();
            return ans;
        }
    }
}


