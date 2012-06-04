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
            
			rule:: rule( const rule &other ) :
			label( other.label ),
            next(0),
            prev(0)
			{
			}
            
			void rule:: grow( parse_node * & Tree, parse_node * &Node )
			{
				assert( Node );
				if( !Tree )
				{
					Tree = Node;
                    Node = NULL;
				}
				else
				{
					if( Tree->terminal )
					{
						const exception excp("Invalid Parse Tree: '%s' is terminal, can't append '%s'", Tree->label.c_str(), Node->label.c_str());
						delete Node;
						delete Tree;
						Tree = NULL;
                        Node = NULL;
						throw excp;
					}
					Tree->children().push_back( Node );
				}
			}
            
            
            
            
		}
        
	}
    
}
