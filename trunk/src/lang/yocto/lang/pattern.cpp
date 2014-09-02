#include "yocto/lang/pattern.hpp"
#include "yocto/ios/ocstream.hpp"

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

        void pattern:: refactor()
        {
            
        }
        
    }
}


