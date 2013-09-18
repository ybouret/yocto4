#include "yocto/rx/compiler.hpp"
#include "yocto/exceptions.hpp"

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/joker.hpp"
#include "yocto/rx/pattern/posix.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/string/tokenizer.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/sequence/vector.hpp"

#include <iostream>

namespace yocto
{
	
#define LBRACK '['
#define RBRACK ']'
	
#define LBRACE '{'
#define RBRACE '}'
	
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
			const char *        curr;
			const char * const  last;
			const pattern_dict *dict;
			int                 depth;
			static const char   fn[];
			
			inline compiler( const string &expr, const pattern_dict *db ) throw() :
			curr( expr.c_str()       ),
			last( curr + expr.size() ),
			dict( db ),
			depth(0)
			{
				
			}
			
			inline ~compiler() throw() {}
			
			//==================================================================
			// sub-expression parsing
			//==================================================================
			inline pattern *sub()
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
							
							auto_ptr<pattern> lhs;
							switch( p->operands.size )
							{
								case 0:  throw excp(fn,"empty left hand side for '|' in ...'%s'", ini);
								case 1:  lhs.reset( p->operands.pop_back() ); break;
								default: lhs.reset( p.yield() ); break;
							}
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
							
							//--------------------------------------------------
							//
							// escaped sequences
							//
							//--------------------------------------------------
						case '\\':
							*p << basic::single::create( sub_esc() );
							break;
							
							
							//--------------------------------------------------
							//
							// special
							//
							//--------------------------------------------------
						case '.':
							*p << posix::dot();
							break;
							
							//--------------------------------------------------
							//
							// group
							//
							//--------------------------------------------------
						case LBRACK:
							*p << grp();
							assert( RBRACK == curr[0] );
							break;
							
							
							//--------------------------------------------------
							//
							// named expr or joker
							//
							//--------------------------------------------------
						case LBRACE:
							braces( *p );
							assert( RBRACE == curr[0] );
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
			inline void jokerize( logical::Operator &p, const char j )
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
			
			//==================================================================
			// escaped sequence in sub expression
			//==================================================================
			inline char sub_esc()
			{
				assert(curr<last); assert( curr[0] == '\\' );
				if( ++curr >= last )
				{
					throw excp( fn, "unfinished escaped sequence");
				}
				const char C = curr[0];
				switch( C )
				{
					case '\\':
					case '(':
					case ')':
					case '[':
					case ']':
					case '|':
					case '?':
					case '+':
					case '*':
					case '{':
					case '}':
					case '.':
						return C;
						
					case 'n': return '\n';
					case 'r': return '\r';
					case 't': return '\t';
						
					case 'x':
						return hex();
						
					default:
						break;
				}
				throw excp( fn, "invalid expression escaped char '%c'", C );
			}
			
			//==================================================================
			// hexadecimal
			//==================================================================
			inline char hex() 
			{
				assert( 'x' == curr[0] );
				
				if( ++curr >= last ) 
					throw excp( fn, "missing first hexa byte");
				const int hi = hex2dec( curr[0] );
				if( hi < 0 ) 
					throw excp( fn, "invalid first hexa byte '%c'", curr[0]);
				
				if( ++curr >= last ) throw excp( fn, "missing second hexa byte");
				const int lo = hex2dec( curr[0] );
				if( lo < 0 ) 
					throw excp( fn, "invalid second hexa byte '%c'", curr[0]);
				
				const uint8_t res = uint8_t( (hi << 4) | lo );
				return res;
			}
			
			//==================================================================
			// new group
			//==================================================================
			inline pattern *grp()
			{
				assert( LBRACK == curr[0] );
				const char *ini = curr;
				if( ++curr >= last )
					goto EOGRP;
				else 
				{
					//-- first char after LBRACK
					auto_ptr<logical::Operator> g;
					char C = curr[0];
					switch( C )
					{
						case ':': return grp_posix();
							
						case '^':
							g.reset( logical::NONE::create() );
							break;
							
						case '-':
							g.reset( logical::OR::create() );
							*g << basic::single::create( '-' );
							break;
							
						default:
							g.reset( logical::OR::create() );
							--curr;
							break;
					}
					
					//-- remaining chars
					while( ++curr < last )
					{
						C  = curr[0];
						switch( C )
						{
								
							case RBRACK:
								//----------------------------------------------
								// End of Group
								//----------------------------------------------
							{
								switch( g->operands.size )
								{
									case 0:
										throw excp( fn, "empty group after ...'%s'", ini );
										
									case 1:
										if( logical::OR::id == g->type )
											return g->operands.pop_back();
										break;
										
									default:
										break;
								}
								return g.yield();
							}
								
							case LBRACK:
								//----------------------------------------------
								// slight recursivity
								//----------------------------------------------
								*g << grp();
								break;
								
							case '\\':
								//----------------------------------------------
								// escaped
								//----------------------------------------------
								*g << basic::single::create( grp_esc() );
								break;
								
								
							case '-':
								//----------------------------------------------
								// range
								//----------------------------------------------
								rng( *g );
								break;
								
							default:
								//----------------------------------------------
								// single char
								//----------------------------------------------
								*g << basic::single::create(C);
								break;
						}
					}
				}
			EOGRP:
				throw excp( fn, "unfinished group after ...'%s'", ini );
			}
			
			inline void rng( logical::Operator &g )
			{
				assert( '-' == curr[0] );
				p_list &ops = g.operands;
				if( ops.size <= 0 ) throw excp(fn,"missing left hand operand for range");
				if( ops.tail->type != basic::single::id )
					throw exception("invalid left hand operand type for range");
				assert( ops.tail->data != NULL );
				const char lower = *static_cast<char *>( ops.tail->data );
				delete ops.pop_back();
				if( ++curr >= last ) throw excp( fn, "unfinished range");
				char upper = curr[0];
				switch( upper )
				{
					case '\\':
						upper = grp_esc();
						break;
						
					case '[':
					case ']':
					case '{':
					case '}':
						throw excp( fn, "invalid right hand side '%c' for range", upper );
						
					default:
						break;
				}
				g << basic::range::create( lower, upper );
			}
			
			//==================================================================
			// escaped sequence in group
			//==================================================================			
			inline char grp_esc()
			{
				assert(curr<last); assert( curr[0] == '\\' );
				if( ++curr >= last )
				{
					throw excp( fn, "unfinished escaped sequence");
				}
				const char C = curr[0];
				switch( C )
				{
					case '\\':
					case '[':
					case ']':
					case '^':
					case '-':
						return C;
						
					case 'n': return '\n';
					case 'r': return '\r';
					case 't': return '\t';
						
					case 'x':
						return hex();
						
					default:
						break;
				}
				throw excp( fn, "invalid group escaped char '%c'", C );
			}
			
			
			//==================================================================
			// posix group [:NAME:]
			//==================================================================
#define YRX_POSIX(ID) if( #ID == key ) return posix::ID()
			inline pattern *grp_posix()
			{
				assert(':' == curr[0] );
				const char *ini = curr-1;
				const char *org = curr+1;
				while( ++curr < last )
				{
					if( RBRACK == curr[0] )
					{
						const char *end = curr-1;
						if( end <= org || end[0] != ':' )
							throw exception("invalid posix group after ...'%s'", ini );
						const string key(org,end-org);
						YRX_POSIX(alnum);
						YRX_POSIX(alpha);
						YRX_POSIX(blank);
						YRX_POSIX(cstring);
						YRX_POSIX(digit);
						YRX_POSIX(dot);
						YRX_POSIX(endl);
						YRX_POSIX(lower);
						YRX_POSIX(punct);
						YRX_POSIX(space);
						YRX_POSIX(upper);
						YRX_POSIX(word);
						YRX_POSIX(xdigit);
						throw exception("unknown [:%s:]'", key.c_str() );
					}
					
				}
				throw exception("unfinished posix group after ...'%s'", ini );
			}
			
			
			//==================================================================
			// braces
			//==================================================================
			inline void braces( logical::Operator &p )
			{
				assert( LBRACE == curr[0] );
				const char *org = ++curr;
				while( curr <= last )
				{
					if( RBRACE == curr[0] )
					{
						const string key(org,curr-org);
						if( 0 == key.size() )
							throw excp( fn, "empty braces after ...'%s'", org-1);
						
						const char C = key[0];
						if( C >= '0' && C <= '9' )
						{
							jokerize2( p.operands, key );
						}
						else 
						{
							if( !dict )
								throw excp(fn, "no database for '%s'", key.c_str() );
							p.operands.push_back( dict->create( key ) );
						}
						
						return;
					}
					++curr;
				}
				throw exception("unfinished braces after ...'%s'", org-1);
			}
			
			//==================================================================
			// braces args for counting
			//==================================================================
			static inline bool is_sep( char C ) throw() { return C == ','; }
			
			void jokerize2( p_list &ops, const string &jk )
			{
				// prepare for joker
				if( ops.size <= 0 )
					throw excp( fn, "missing arguments before braces");
				auto_ptr<pattern> q( ops.pop_back() ); 
				
				// parse args
				vector<string,memory::pooled::allocator> arr(4,as_capacity);
				tokenizer::split( arr, jk, is_sep );
				
				// create joker
				switch( arr.size() )
				{
					case 1: {
						const size_t num = strconv::to_size( arr[1], fn );
						ops.push_back( joker::counting( q.yield(), num, num ) ); }
						break;
						
					case 2: {
						const size_t nmin = strconv::to_size( arr[1], fn );
						const size_t nmax = strconv::to_size( arr[2], fn ); 
						if( nmax < nmin ) throw excp( fn, "invalid braces args" );
						ops.push_back( joker::counting( q.yield(), nmin, nmax ) ); }
						break;
						
					default:
						throw excp(fn, "bad argument count in braces" );
				}
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
		};
		
		const char compiler::fn[] = "regex::compiler exception";
		
		
		
		pattern *compile( const string &expr, const pattern_dict *db )
		{
			compiler          cc( expr, db );
			auto_ptr<pattern> pp( cc.sub() );
			if( cc.depth > 0 )
				throw compiler::excp( compiler::fn, "unfinished sub-expression in '%s'", expr.c_str() );
			pp->optimize();
			return pp.yield();
		}
		
		pattern *compile( const char *expr, const pattern_dict *db )
		{
			const string xp( expr );
			return compile( xp, db);
		}
	}
	
}
