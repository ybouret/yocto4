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
			
			syntax::result terminal:: match( lexer &lxr, source &src, lexemes &stk )
			{
				
				//--------------------------------------------------------------
				//
				// register
				//
				//--------------------------------------------------------------
				lexemes local_stk;
				
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
					if( src.is_active() )
					{
						//------------------------------------------------------
						//-- syntax error
						//------------------------------------------------------
						char C = src.peek()->data;
						exception excp( "syntax error", "'%c' in ", C );
						unwind( excp );
						throw excp;
					}
					else
					{
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
						std::cerr << "match '" << name << "'=<" << (*lx) << ">" << std::endl;
						local_stk.push_back( lx );
						stk.merge_back( local_stk );
						return syntax::success;
					}
					else 
					{
						//- - - - - - - -
						//-- not matching
						//- - - - - - - -
						lxr.unget( lx );
						return syntax::unexpected;						
					}
				}
			}
			
			
			
		}
		
	}
	
}
