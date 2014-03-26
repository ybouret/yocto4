#include "yocto/lang/syntax/rule.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	namespace lang 
	{
		namespace syntax 
		{
            
			rule:: ~rule() throw() {}
            
            
			rule:: rule( const string &id ) : 
			label(id),
            next(0),
            prev(0)
			{}
            
			
            void rule:: check( const parse_node *Tree)
            {
                if( Tree && Tree->terminal )
                    throw exception("invalid terminal tree <%s> for rule <%s>", Tree->label.c_str(), label.c_str() );
            }

			void rule:: grow( parse_node * & Tree, parse_node * &Node ) throw()
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
            
            
            rules:: rules() throw() {}
            static inline void __delete_rule( rule *r ) throw() { assert(r); delete r; }
            rules:: ~rules() throw() { delete_with( __delete_rule ); }
            
            
		}
        
	}
    
}
