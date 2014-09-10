#include "yocto/lingua/syntax/rules.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
     
        namespace syntax
        {
            rule:: ~rule() throw() {}
            
            
			rule:: rule( const string &id ) :
			label(id),
            next(0),
            prev(0)
			{}
            
            void rule:: check( const xnode *tree )
            {
                if( tree && tree->terminal )
                    throw exception("invalid terminal tree <%s> for rule <%s>", tree->label.c_str(), label.c_str() );
            }

            
            
			void rule:: grow( xnode * & Tree, xnode * &Node ) throw()
			{
				assert( Node );
                
				if( !Tree )
				{
					Tree = Node;
                    Node = NULL;
				}
				else
				{
					assert(! Tree->terminal );
					Tree->children().push_back( Node );
                    Node->parent = Tree;
				}
			}
            
            r_list:: r_list() throw() {}
            static inline void __delete_rule( rule *r ) throw() { assert(r); delete r; }
            r_list:: ~r_list() throw() { delete_with( __delete_rule ); }

            
        }
    }
}

