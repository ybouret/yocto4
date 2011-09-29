#include "yocto/rx/syntactic/terminal.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			terminal:: ~terminal() throw() {}
			
			terminal:: terminal( const string &n) : 
			rule( ID, n )
			{
			}
			
			terminal * terminal:: create( const string &n )
			{
				return new terminal( n );
			}
			
			terminal * terminal:: create( const char *txt )
			{
				const string n( txt );
				return new terminal( n );
			}
			
			syntax::result terminal:: match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				
				std::cerr << "parse.terminal '" << name << "'" << std::endl;
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
					std::cerr << ".not found" << std::endl;
					if( src.is_active() )
					{
						//------------------------------------------------------
						//-- syntax error
						//------------------------------------------------------
						std::cerr << "..error!!" << std::endl;
						char C = src.peek()->data;
						exception excp( "syntax error", "'%c' in ", C );
						unwind( excp );
						throw excp;
					}
					else
					{
						std::cerr << "..end-of-input" << std::endl;
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
						std::cerr << ".match '" << name << "'=<" << (*lx) << ">" << std::endl;
						s_node *node = s_node::create(lx);
						if( tree )
						{
							switch( tree->type )
							{
								case s_node::terminal:
									s_node::destroy(node,src.char_pool);
									throw yocto::exception("bad tree!");
									
								case s_node::internal:
									break;
							}
							
						}
						else
						{
							tree = node;
						}
						
						return syntax::success;
					}
					else 
					{
						//- - - - - - - -
						//-- not matching
						//- - - - - - - -
						std::cerr << ".not matching: '" << lx->label << "'" << std::endl;
						lxr.unget( lx );
						return syntax::unexpected;						
					}
				}
			}
			
			
			
		}
		
	}
	
}
