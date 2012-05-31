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
            label(id) 
            {}
            
            rule:: rule( const rule &other ) :
            label( other.label )
            {
            }
            
            void rule:: grow( parse_node * & Tree, parse_node *node )
			{
				assert( node );
				if( !Tree )
				{
					Tree = node;
				}
				else
				{
					if( Tree->terminal )
					{
						const exception excp("Invalid Parse Tree: '%s' is terminal, can't append '%s'", Tree->label.c_str(), node->label.c_str());
						delete node;
						delete Tree;
						Tree = NULL;
						throw excp;
					}
					Tree->children().push_back( node );
				}
			}
            
        }

    }

}
