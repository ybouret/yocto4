#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	namespace regex
	{
		
				
		lexeme * lexer::operator()( source &src )
		{
			//------------------------------------------------------------------
			// find a matching rule
			//------------------------------------------------------------------
			lexical::rule *r = rules_.head;
			for( ; r; r = r->next )
			{
				if( r->motif->accept(src) )
				{
					goto FIND_BEST;
				}
			}
			return NULL; //-- no match !
			
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
				
				
				lexeme      *lx = lexeme::create( *best );	
				assert( 0 == best->motif->size );
				const size_t nx = lx->size;
				assert( src.in_cache() >= nx );
				src.skip(nx);
				return lx;
			}
		}
		
	}
}