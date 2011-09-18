#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	namespace regex
	{
		
		lexeme:: lexeme( token &tk, const string &id ) throw() :
		token(),
		next(NULL),
		prev(NULL),
		label( id )
		{
			swap_with( tk );
		}
		
		lexeme:: ~lexeme() throw()
		{
			
		}
		
		lexeme * lexeme:: create( token &tk, const string &id )
		{
			return new lexeme( tk, id );
		}
		
		void     lexeme:: destroy( lexeme *lx, source &src ) throw()
		{
			lx->back_to( src.pool );
			delete lx;
		}
		
		void     lexeme:: destroy( lexeme *lx ) throw()
		{
			delete lx;
		}
		
		lexemes:: lexemes() throw() {}
		lexemes:: ~lexemes() throw() { delete_with( lexeme::destroy ); }
		
		void lexemes:: to( source &src ) throw()
		{
			delete_with<source &>( lexeme::destroy, src );
		}
		
		
		lexeme * lexer::operator()( source &src )
		{
			//------------------------------------------------------------------
			// find a matching rule
			//------------------------------------------------------------------
			lexical::rule *r = rules.head;
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
				
				
				lexeme      *lx = lexeme::create( *(best->motif), best->label );	
				assert( 0 == best->motif->size );
				const size_t nx = lx->size;
				assert( src.in_cache() >= nx );
				src.skip(nx);
				return lx;
			}
		}
		
	}
}