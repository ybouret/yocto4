#include "yocto/lang/parser.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        parser:: ~parser() throw()
        {
        }
        
        parser:: parser( const string &id ) :
        L(id),
        G(id),
        scan( L.first() )
        {
            
        }

        
        regex::pattern_dict & parser:: dict()
        {
            return scan.dict();
        }
        
        void parser:: no_dict() throw()
        {
            scan.no_dict();
        }
        
        
        void parser:: term( const string &id, const string &expr, syntax::node_property  )
        {
            
        }

        
    }

}

