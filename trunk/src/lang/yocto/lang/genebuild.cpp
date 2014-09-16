#include "yocto/lang/generator.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/list.hpp"

#include <cstdlib>

namespace yocto
{
    namespace lang
    {
        namespace
        {
            
            
        }
        
        void generator:: process( ios::istream &fp )
        {
            //__________________________________________________________________
            //
            // Get user rules
            //__________________________________________________________________
            initialize();
            auto_ptr<XNode> Tree( this->run(fp) );
            assert( Tree.is_valid() );
            
            //__________________________________________________________________
            //
            // rewrite stuff
            //__________________________________________________________________
            XNode *root = Tree.__get();
            rewrite(root);
            root->graphviz("xnode.dot");
            system("dot -Tpng -o xnode.png xnode.dot");
            
            
            //__________________________________________________________________
            //
            // generate parser
            //__________________________________________________________________
            auto_ptr<parser> P( new parser("generated","scanner") );
            
            

            
        }
        
    }
    
}
