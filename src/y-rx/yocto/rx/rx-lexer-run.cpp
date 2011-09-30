#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"
#include <iostream>

namespace yocto
{
	namespace regex
	{
		
		
		
		lexeme  *lexer:: consume( source &src )
		{
			//------------------------------------------------------------------
			// find a matching rule
			//------------------------------------------------------------------
		FIND_RULE:
			lexical::rule *r = rules_.head;
			for( ; r; r = r->next )
			{
				if( r->motif->accept(src) )
				{
					goto FIND_BEST;
				}
			}
			return NULL; //-- no match || no source data
			
			//------------------------------------------------------------------
			// find the best possible rule
			//------------------------------------------------------------------
		FIND_BEST:
			{
				lexical::rule *best = r;     //-- register best rule
				src.uncpy( *(best->motif) ); //-- restore src for next search
				
				for( r=r->next; r; r=r->next )
				{
					if( r->motif->accept(src) )
					{
						//------------------------------------------------------
						// we have a new solution
						//------------------------------------------------------
						if( best->motif->size >= r->motif->size )
						{
							//--------------------------------------------------
							//-- too late !
							//--------------------------------------------------
							src.unget( *(r->motif) ); //... and keep on probing
						}
						else
						{
							//--------------------------------------------------
							//-- new best !	
							//--------------------------------------------------
							best->motif->sendto( src );  //... forget it
							best = r;                    // register new best
							src.uncpy( *(best->motif) ); //.. and keep on probing
						}
						
						
					}
				}
				
				if( ! best->check( *(best->motif) ) )
				{
					assert( src.in_cache() >= best->motif->size );
					src.skip(best->motif->size);
					src.drop( *(best->motif)  );
					goto FIND_RULE;
				}
												
				//--------------------------------------------------------------
				// process the best rule
				//--------------------------------------------------------------
				lexeme      *lx = lexeme::create( *best );	
				assert( 0 == best->motif->size );
				
				//--------------------------------------------------------------
				// forward the source
				//--------------------------------------------------------------
				const size_t nx = lx->size;
				assert( src.in_cache() >= nx );
				src.skip(nx);
				
				//--------------------------------------------------------------
				// all done: TODO best->motif->sendto( src ) ?
				//--------------------------------------------------------------
				//std::cerr << "-- best lex=" << lx->label << std::endl;
				return lx;
			}
		}
		
		lexeme * lexer:: lookahead( source &src )
		{
			if( cache.size > 0 )
			{
				return cache.pop_front();
			}
			else
			{
				return consume(src);
			}
			
		}
		
	}
}
