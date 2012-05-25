#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	namespace lang
	{
        
		namespace lexical
		{
			lexeme * scanner:: next_lexeme( regex::source &src )
			{
                
				//--------------------------------------------------------------
				//
				// find a rule that produces a lexeme
				//
				//--------------------------------------------------------------
				while(true)
				{
					//----------------------------------------------------------
					// find the first accepting rule => best_rule
					//----------------------------------------------------------
					rule    *best_rule = NULL;
					rule    *r = rules_.head;
					while(r)
					{
						if( r->motif->accept(src) )
						{
							best_rule = r;
							break;
						}
						r=r->next;
					}
                    
					if( !best_rule )
					{
                        //------------------------------------------------------
						//-- no accepting pattern !
                        //------------------------------------------------------
                        const regex::t_char *ch = src.peek();
                        
                        if( ch )
                        {
                            //--------------------------------------------------
                            // source is still active !
                            //--------------------------------------------------
                            throw exception("%u:%s: unexpected char '%c'", unsigned(line), name.c_str(), ch->data);
                        }
                        else 
                        {
                            //--------------------------------------------------
                            // EOF
                            //--------------------------------------------------
                            return NULL;
                        }
                        
                    }			
                   
					//----------------------------------------------------------
					// scan other rules for a better match
					//----------------------------------------------------------
					r = r->next;
					src.uncpy( *(best_rule->motif) ); //! for subsequent researched
					while( r )
					{
						if( r->motif->accept(src) )
						{
							//--------------------------------------------------
							// we have a challenger !
							//--------------------------------------------------
							if( r->motif->size > best_rule->motif->size )
							{
								//----------------------------------------------
								// new winner !
								//----------------------------------------------
								best_rule->motif->clear();        //! forget current winner
								best_rule = r;                    //! and upgrade
								src.uncpy( *(best_rule->motif) ); //! for next rule
							}
							else 
							{
								//----------------------------------------------
								// too late !
								//----------------------------------------------
								src.unget( *(r->motif) ); //! for next rule
								r->motif->clear();
							}
						}
						r=r->next;
					}
                    
					//----------------------------------------------------------
					// skip in the source
					//----------------------------------------------------------
					assert( src.in_cache() >= best_rule->motif->size );
					src.skip(best_rule->motif->size);
                    
					//----------------------------------------------------------
					// check is we keep the lexeme
					//----------------------------------------------------------
					if( best_rule->produce() )
					{
						//-- create the lexeme
						lexeme *lx = new lexeme( best_rule->label, line );
                        
						//-- steal the token
						lx->swap_with( *(best_rule->motif) );
						best_rule->motif->clear();
                        
						//-- done
						return lx;
					}
					else
					{
						best_rule->motif->clear();
					}
                    
					//----------------------------------------------------------
					// ready for next producing rule...
					//----------------------------------------------------------
				}
				return NULL; // never get there
			}
            
		}
        
	}
    
}

