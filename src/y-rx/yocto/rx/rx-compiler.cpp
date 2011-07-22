#include "yocto/rx/compiler.hpp"
#include "yocto/exceptions.hpp"

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/joker.hpp"

#include "yocto/auto-ptr.hpp"


namespace yocto
{
	
	namespace regex
	{
		////////////////////////////////////////////////////////////////////////
		//
		// the compiler class 
		//
		////////////////////////////////////////////////////////////////////////
		
		class compiler
		{
		public:
			typedef imported::exception excp;
			const char *       curr;
			const char * const last;
			const pattern_db  *dict;
			int                depth;
			static const char  fn[];
			
			compiler( const string &expr, const pattern_db *db ) throw() :
			curr( expr.c_str()       ),
			last( curr + expr.size() ),
			dict( db ),
			depth(0)
			{
				
			}
			~compiler() throw() {}
			
			//==================================================================
			// sub-expression parsing
			//==================================================================
			pattern *sub()
			{
				//--------------------------------------------------------------
				// initial AND
				//--------------------------------------------------------------
				auto_ptr<logical::Operator> p(logical::AND::create());
				const char                 *ini = curr;
				
				//--------------------------------------------------------------
				// main loop
				//--------------------------------------------------------------
				while( curr < last )
				{
					const char C = curr[0];
					switch( C )
					{
							
							//--------------------------------------------------
							//
							// sub-expressions
							//
							//--------------------------------------------------
						case '(': {
							//--------------------------------------------------
							//-- start sub expression
							//--------------------------------------------------
							++depth; //-- global depth
							++curr;  //-- skip LPAREN
							p->operands.push_back( sub() );
						} 
							break;
							
						case ')':
							//--------------------------------------------------
							//-- end sub expression
							//--------------------------------------------------
							if( --depth < 0 ) 
								throw excp( fn, "no sub-expression to end after '%s'", ini);
							goto END_SUB;
							
							
							//--------------------------------------------------
							//
							// alternation
							//
							//--------------------------------------------------
						case '|': {
							auto_ptr<pattern> lhs( p.yield() );
							p.reset( logical::OR::create() );
							++curr; //-- skip ALT
							auto_ptr<pattern> rhs( sub() );
							p->operands.push_back( lhs.yield() );
							p->operands.push_back( rhs.yield() );
						}
							goto END_SUB;
														
							//--------------------------------------------------
							//
							// jokers
							//
							//--------------------------------------------------
						case '?':
						case '+':
						case '*':
							jokerize(*p,C);
							break;
							
						default: 
							//--------------------------------------------------
							//-- a single simple char
							//--------------------------------------------------
							*p << basic::single::create( C );
					}
					
					++curr;
				}
				
				//--------------------------------------------------------------
				// analyze end of sub-expression
				//--------------------------------------------------------------
			END_SUB:
				switch( p->operands.size )
				{
				    case 0:
						throw excp(fn,"empty sub-expression after '%s'", ini );
						
					case 1:
						return p->operands.pop_back();
						
					default:
						break;
				}
				return p.yield();
			}
			
			//==================================================================
			// extract last pattern for the joker
			//==================================================================
			void jokerize( logical::Operator &p, const char j )
			{
				if( p.operands.size <= 0 ) 
					throw excp( fn, "no previous pattern for joker '%c'", j );
				pattern *q = p.operands.pop_back();
				switch( j )
				{
					case '+':
						p.operands.push_back( joker::at_least(q,1) );
						break;
						
					case '?':
						p.operands.push_back( joker::counting(q,0,1) );
						break;
						
					case '*':
						p.operands.push_back( joker::at_least(q,0) );
						break;
						
					default:
						delete q;
						throw excp( fn, "invalid joker '%c'", j );
				}
				
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
		};
		
		const char compiler::fn[] = "regex::compiler exception";
		
		
		
		pattern *compile( const string &expr, const pattern_db *db )
		{
			compiler          cc( expr, db );
			auto_ptr<pattern> pp( cc.sub() );
			if( cc.depth > 0 )
				throw compiler::excp( compiler::fn, "unfinished sub-expression in '%s'", expr.c_str() );
			return pp.yield();
		}
		
		pattern *compile( const char *expr, const pattern_db *db )
		{
			const string xp( expr );
			return compile( xp, db);
		}
	}
	
}
