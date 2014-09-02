#include "yocto/lang/pattern/compiler.hpp"

#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"


#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	namespace lang
	{
#define YRX_VERBOSE 0
        
#if defined(YRX_VERBOSE) && (YRX_VERBOSE == 1 )
#define YRX(CODE) do { CODE; } while(false)
#else
#define YRX(CODE)
#endif
        
        
#define LBRACE '{'
#define RBRACE '}'
        
#define LBRACK '['
#define RBRACK ']'
        
        namespace
        {
            
            static const char fn[] = "lingua::compile";
            
            ////////////////////////////////////////////////////////////////////
            //
			//
			// compile posix
			//
            //
			////////////////////////////////////////////////////////////////////
            static inline
            pattern * compile_posix( const char * &curr, const char * last)
            {
                YRX(std::cerr << "[GRP] posix" << std::endl);
                
                //==============================================================
                // extract the name
                //==============================================================
                assert( ':' == curr[0] );
                if( ++curr >= last )
                    throw exception("%s(Unfinished Posix)",fn);
                const char *org = curr;
                while(curr<last)
                {
                    if( ':' == curr[0] )
                    {
                        ++curr;
                        if( curr[0]  != RBRACK )
                            throw exception("%s(Invalid Posix after'%s'",fn,org-2);
                        
                        assert(curr>org);
                        const string id(org,(curr-org)-1);
                        YRX(std::cerr << "[GRP] id=<" << id << ">" << std::endl);
#define YRX_POSIX(ID) if( id == #ID ) return posix::ID()
                        
                        YRX_POSIX(upper);
                        YRX_POSIX(lower);
                        YRX_POSIX(digit);
                        YRX_POSIX(alpha);
                        YRX_POSIX(alnum);
                        YRX_POSIX(xdigit);
                        YRX_POSIX(blank);
                        YRX_POSIX(space);
                        YRX_POSIX(punct);
                        
                        YRX_POSIX(word);
                        YRX_POSIX(endl);
                        YRX_POSIX(cstring);
                        
                        throw exception("%s(Unhandled Posix '%s')", fn, id.c_str());
                        
                        //======================================================
                        // return whatever
                        //======================================================
                    }
                    ++curr;
                }
                throw exception("%s(Unfinished Posix after '%s')",fn,org-2);
            }
            
            
			////////////////////////////////////////////////////////////////////
            //
			//
			// hexa
			//
            //
			////////////////////////////////////////////////////////////////////
			static inline
            uint8_t parse_hexa(const char * &curr, const char * last)
			{
				assert('x' == curr[0]);
				if(++curr>=last)
					throw exception("%s(Missing First Hexa Char)",fn);
				if( !isxdigit(curr[0]) )
					throw exception("%s(Invalid First Hexa Byte)",fn);
                
				int ans = hex2dec(curr[0]) << 4;
                
				if(++curr>=last)
					throw exception("%s(Missing Second Hexa Char)",fn);
				if( !isxdigit(curr[0]) )
					throw exception("%s(Invalid Second Hexa Char)",fn);
				ans |= hex2dec(curr[0]);
                
				return uint8_t(ans);
			}
            
            ////////////////////////////////////////////////////////////////////
            //
			//
			// escaped sequence in group
			//
            //
			////////////////////////////////////////////////////////////////////
            static inline
            pattern *group_esc(const char * &curr, const char * last)
            {
                assert( '\\' == curr[0] );
                if(++curr>=last)
                    throw exception("%s(Unfinished Group Escaped Sequence)",fn);
                
                char C = curr[0];
                YRX(std::cerr << "[GRP] Escaped <" << C << ">" << std::endl);
                switch(C)
                {
                    case 'x':
                        C = char(parse_hexa(curr, last));
                        break;
                        
                        //------------------------------------------------------
                        //litteral
                        //------------------------------------------------------
                    case '\\':
                    case ':':
                    case '[':
                    case ']':
                    case '^':
                    case '-':
                        break;
                        
                        //-------------------------------------------------
                        // control escaped
                        //-------------------------------------------------
                    case 'n': C='\n'; break;
                    case 'r': C='\r'; break;
                    case 't': C='\t'; break;
                        
                    default:
                        throw exception("%s(Unknown Group Escaped Sequence '%c')",fn,C);
                }
                return single::create(C);
            }
            
            ////////////////////////////////////////////////////////////////////
            //
			//
			// next single in group
			//
            //
			////////////////////////////////////////////////////////////////////
            static inline
            pattern *next_single( const char * &curr, const char *last )
            {
                assert( '-' == curr[0] );
                if(++curr>=last)
                    throw exception("%s(Unfinished Range)",fn);
                
                const char C = curr[0];
                switch(C)
                {
                    case '\\':
                        return group_esc(curr, last);
                        
                    case '[':
                    case ']':
                        throw exception("%s(Invalid Range Char '%c')", fn, C);
                        
                    default:
                        break;
                }
                return single::create(C);
            }
            
			////////////////////////////////////////////////////////////////////
			//
            //
			// group
			//
            //
			////////////////////////////////////////////////////////////////////
			static inline
            pattern *compile_group(const char * &curr, const char * last)
			{
				auto_ptr<logical> p(0);
				YRX(std::cerr << "[GRP] Start New Group" << std::endl);
                
				assert( LBRACK == curr[0] );
                
				//==============================================================
				//
				// do we have a caret ?
				//
				//==============================================================
                
                
				//-- skip LBRACK
				if(++curr>=last)
					throw exception("%s(Unfinished Group)", fn);
                
				//-- test caret
				if( '^' == curr[0] )
				{
					p.reset( NOT::create() );
					++curr; // skip it
					if(curr>=last)
						throw exception("%s(Unfinihsed group after ^)", fn );
					YRX(std::cerr << "[GRP] <<NOT>>" <<std::endl);
				}
				else
				{
                    if( ':' == curr[0] )
                        return compile_posix(curr,last);
                    
					p.reset( OR::create() );
				}
                
                
				assert(p.is_valid());
				//==============================================================
				//
				// do we have a '-' ?
				//
				//==============================================================
				assert(curr<last);
				if( '-' == curr[0] )
				{
					p->append( single::create('-') );
					++curr;
				}
                
				//=============================================================
				//
				// process the rest
				//
				//==============================================================
				while(curr<last)
				{
					char C = curr[0];
                    
					switch(C)
					{
                        case RBRACK:
                            YRX( std::cerr << "[GRP] End" << std::endl );
                            goto FINISHED;
                            
                            //--------------------------------------------------
                            //
                            // recursive for multiple grouping
                            //
                            //--------------------------------------------------
                        case LBRACK:
                            p->append( compile_group(curr, last) );
                            assert(RBRACK==curr[0]);
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // Escaped Sequence
                            //
                            //--------------------------------------------------
                        case '\\':
                            p->append( group_esc(curr, last) );
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // Range
                            //
                            //--------------------------------------------------
                        case '-':
                            YRX(std::cerr << "[GRP] Build range" << std::endl);
                        {
                            if(p->operands.size<=0)
                                throw exception("%s(Range: No Left Symbol)",fn);
                            if(p->operands.tail->type != single::tag)
                                throw exception("%s(Range: Bad Left Type)",fn);
                            
                            pattern *q = p->remove();
                            assert(q->self);
                            const int lower = static_cast<single *>(q->self)->value;
                            delete q;
                            
                            q = next_single(curr, last);
                            assert(q->self);
                            const int upper = static_cast<single *>(q->self)->value;
                            delete q;
                            YRX(std::cerr << "[GRP] New range " << char(lower) << ", " << char(upper) << std::endl);
                            p->append( range::create(lower,upper) );
                            ++curr;
                        }
                            break;
                            
                            //--------------------------------------------------
                            //
                            // generic char
                            //
                            //--------------------------------------------------
                        default:
                            YRX(std::cerr << "[GRP] Append single '" << C << "'" << std::endl);
                            p->append( single::create(C) );
                            ++curr;
                            break;
					}
				}
				throw exception("%s(Unfinished Group)", fn);
                
            FINISHED:
				if(p->operands.size<=0)
					throw exception("%s(Empty Group)", fn );
                
                //p->refactor();
                return pattern::optimize(p.yield());
			}
            
            
			///////////////////////////////////////////////////////////////////
			//
            //
			// braces
			//
            //
			///////////////////////////////////////////////////////////////////
			static inline
            string scan_braces(const char   * &curr,
                               const char   * last)
			{
				assert( LBRACE == curr[0] );
				const char *org = ++curr;
				while(curr<last)
				{
					if(RBRACE==curr[0])
					{
						return string(org,curr-org);
					}
					++curr;
				}
				throw exception("%s(Unfinished Braces)",fn);
			}
            
			static inline
            pattern *compile_xp(const char   * &curr,
                                const char   * last,
                                const p_dict *dict,
                                size_t       &depth )
			{
                
				auto_ptr<logical> p( AND::create() );
				while(curr<last)
				{
					char C = *curr;
					switch(C)
					{
                            //--------------------------------------------------
                            //
                            // SubExpressions
                            //
                            //--------------------------------------------------
                        case '(':
                            ++depth;
                            YRX(std::cerr << "[RXP] Start SubExpression/Depth=" << depth << std::endl);
                            ++curr;
                            p->append( compile_xp(curr,last,dict,depth) );
                            break;
                            
                        case ')':
                            YRX(std::cerr << "[RXP] Finish SubExpression/Depth=" << depth << std::endl);
                            if(depth<=0) throw exception("%s(Unexpected Right Paren)",fn);
                            --depth;
                            ++curr;
                            goto FINISHED;
                            
                            //--------------------------------------------------
                            //
                            // Alternation
                            //
                            //--------------------------------------------------
                        case '|': {
                            YRX(std::cerr << "[RXP] Alternation/Depth=" << depth << std::endl);
                            ++curr;
                            auto_ptr<pattern>  lhs( p.yield() );
                            auto_ptr<pattern>  rhs( compile_xp(curr, last, dict, depth));
                            p.reset( OR::create() );
                            p->append(lhs.yield());
                            p->append(rhs.yield());
                        }
                            goto FINISHED;
                            
                            //--------------------------------------------------
                            //
                            // Jokers
                            //
                            //--------------------------------------------------
                        case '?':
                            YRX(std::cerr << "[RXP] joker '?'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '?')",fn);
                            else
                            {
                                p->append( optional::create(p->remove() ));
                            }
                            ++curr;
                            break;
                            
                        case '+':
                            YRX(std::cerr << "[RXP] joker '+'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '+')",fn);
                            else
                            {
                                p->append( one_or_more(p->remove() ));
                            }
                            ++curr;
                            break;
                            
                        case '*':
                            YRX(std::cerr << "[RXP] joker '*'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '*')",fn);
                            else
                            {
                                p->append( zero_or_more(p->remove() ));
                            }
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // bracket
                            //
                            //--------------------------------------------------
                        case LBRACK:
                            p->append(compile_group(curr,last));
                            assert(RBRACK==curr[0]);
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // braces
                            //
                            //--------------------------------------------------
                        case LBRACE:
						{
							const string content( scan_braces(curr, last) );
							assert(RBRACE==curr[0]);
							++curr;
							YRX(std::cerr << "[RXP] [BRACES] {" << content << "}" << std::endl);
							if(content.size()<=0)
								throw exception("%s(Empty Braces)",fn);
							if(isdigit(content[0]))
							{
								//----------------------------------------------
								// assume counting joker
								//----------------------------------------------
								if( p->operands.size <= 0 )
									throw exception("%s(No SubExpression before Braces)",fn);
								const char *orgA = &content[0];
								const char *endA = orgA;
								const char *endB = orgA + content.size();
								bool        comma = false;
								while(endA<endB)
								{
									if(*endA==',')
									{
										comma = true;
										break;
									}
									++endA;
								}
								const string astr(orgA,endA-orgA);
								const size_t a = strconv::to<size_t>(astr,"First Brace Number");
								YRX( std::cerr << "\ta=" << astr << std::endl);
								if(comma)
								{
									const char *orgB = endA+1;
									if(orgB<endB)
									{
										const string bstr(orgB,endB-orgB);
										const size_t b = strconv::to<size_t>(bstr,"Second Brace Number");
										YRX(std::cerr << "\t=> between " <<a << " and " << b << " times" << std::endl);
										p->append( counting::create( p->remove(), a, b));
									}
									else
									{
										YRX(std::cerr<<"\t=> at least " << a << " times" <<std::endl);
										p->append( at_least::create( p->remove(), a));
									}
								}
								else
								{
									YRX(std::cerr << "\t=> exactly " << a << " times" << std::endl);
									p->append( counting::create( p->remove(), a, a));
								}
							}
							else
							{
								//----------------------------------------------
								// assume named joker from dictionary
								//----------------------------------------------
								if(!dict)
									throw exception("%s(no dict for {%s})", fn, content.c_str());
								p->append( dict->create(content) );
							}
						}
                            break;
                            
                            //--------------------------------------------------
                            //
                            // escaped char
                            //
                            //--------------------------------------------------
                        case '\\':
                            if(++curr>=last)
                                throw exception("%s(Unfinished Escaped Sequence)", fn);
                            C = curr[0];
                            YRX( std::cerr << "[RXP] Escaped Sequence <" << C << ">" << std::endl);
                            switch(C)
						{
                            case 'x':
                                C = char(parse_hexa(curr,last));
                                break;
                                
                                //-------------------------------------------------
                                // simple escaped
                                //-------------------------------------------------
                            case '\\':
                            case '[':
                            case ']':
                            case '.':
                            case '{':
                            case '}':
                            case '+':
                            case '*':
                            case '?':
                            case '(':
                            case ')':
                            case '|':
                            case '"':
                                break;
                                
                                //-------------------------------------------------
                                // control escaped
                                //-------------------------------------------------
                            case 'n': C='\n'; break;
                            case 'r': C='\r'; break;
                            case 't': C='\t'; break;
                                
                            default:
                                throw exception("%s(Unknown Escaped Sequence '%c')", fn, C);
						}
                            ++curr;
                            p->append( single::create(C) );
                            break;
                            
                            //--------------------------------------------------
                            //
                            // specific chars
                            //
                            //--------------------------------------------------
                        case '.':
                            p->append( posix::dot() );
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // single chars
                            //
                            //--------------------------------------------------
                        default:
                            //--------------------------------------------------
                            // a new single char
                            //--------------------------------------------------
                            YRX(std::cerr << "[RXP] append single '" << C << "'" << std::endl);
                            p->append( single::create(C) );
                            ++curr;
					}
				}
                
				if(depth>0)
					throw exception("%s(Unfinished SubExpression)",fn);
                
            FINISHED:
				assert( p.is_valid() );
				if(p->operands.size<=0)
					throw exception("%s(Empty SubExpression)",fn);
                
                //p->refactor();
                return pattern::optimize(p.yield());
            }
            
            
		}
        
        
        
		pattern *compile( const string &expr, const p_dict *dict)
		{
            const char  *curr = expr.c_str();
            const char  *last = curr + expr.size();
            size_t       depth = 0;
            
			return compile_xp(curr, last, dict,depth);
		}
        
		pattern *compile( const char   *expr, const p_dict *dict)
		{
			const string Expr(expr);
			return compile(Expr,dict);
		}
        
	}
}
