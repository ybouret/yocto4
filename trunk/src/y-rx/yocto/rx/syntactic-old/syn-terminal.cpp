#include "yocto/rx/syntactic/terminal.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			terminal:: ~terminal() throw() {}
			
			terminal:: terminal( const string &n, bool k) : 
			rule( ID, n ),
			keeping( k )
			{
			}
			
			terminal * terminal:: create( const string &n , bool k)
			{
				return new terminal(n,k);
			}
			
			rule * terminal:: clone() const 
			{
				std::cerr << ".clone.teminal='" << name << "'" << std::endl;
				return new terminal(name,keeping); 
			}
			
			terminal * terminal:: create( const char *txt, bool k)
			{
				const string n( txt );
				return new terminal(n,k);
			}
			
			syntax::result terminal:: match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				space(std::cerr) << "?TERM='" << name << "'" << std::endl;
				check(tree);
				//--------------------------------------------------------------
				//
				// find lookahead
				//
				//--------------------------------------------------------------
				lexeme *lx = lxr.lookahead( src );
				if( !lx )
				{
					//----------------------------------------------------------
					//
					//-- no lexeme
					//
					//----------------------------------------------------------
					space(std::cerr) << ".not found" << std::endl;
					if( src.is_active() )
					{
						//------------------------------------------------------
						//-- syntax error
						//------------------------------------------------------
						space(std::cerr) << "..error!!" << std::endl;
						char C = src.peek()->data;
						exception excp( "syntax error", "'%c' in ", C );
						unwind( excp );
						throw excp;
					}
					else
					{
						space(std::cerr) << "..end-of-input" << std::endl;
						return syntax::nothing;
					}
					
				}
				else
				{
					//----------------------------------------------------------
					//-- found a lexeme
					//----------------------------------------------------------
					if( lx->label == name )
					{
						//- - - - - - -
						//-- matching !
						//- - - - - - -
						space(std::cerr) << ".match '" << name << "'=<" << (*lx) << ">" << std::endl;
						s_node *node = s_node::create(lx,*this);
						if( tree )
						{
							assert( s_node::internal == tree->type );
							tree->append( node );
						}
						else
						{
							tree = node;
						}
						if( !keeping ) lx->back_to( src.char_pool );
						return syntax::success;
					}
					else 
					{
						//- - - - - - - -
						//-- not matching
						//- - - - - - - -
						space(std::cerr) << ".not matching: '" << lx->label << "'" << std::endl;
						lxr.unget( lx );
						return syntax::unexpected;						
					}
				}
			}
			
			
			
		}
		
	}
	
}
