#include "yocto/lingua/pattern/compiler.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/auto-ptr.hpp"
#include "yocto/exception.hpp"

#include <cstdlib>

#define YRX_VERBOSE 1

#if defined(YRX_VERBOSE) && (YRX_VERBOSE == 1 )
#define YRX(CODE) do { CODE; } while(false)
#else
#define YRX(CODE)
#endif


#define LBRACE '{'
#define RBRACE '}'

namespace yocto
{
    namespace lingua
    {
        
        namespace
        {
            static const char fn[] = "lingua::compile";

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
                            YRX(std::cerr << "[XP] Start SubExpression/Depth=" << depth << std::endl);
                            ++curr;
                            p->append( compile_xp(curr,last,dict,depth) );
                            break;
                            
                        case ')':
                            YRX(std::cerr << "[XP] Finish SubExpression/Depth=" << depth << std::endl);
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
                            YRX(std::cerr << "[XP] Alternation/Depth=" << depth << std::endl);
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
                            YRX(std::cerr << "[XP] joker '?'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '?')",fn);
                            else
                            {
                                p->append( optional::create(p->operands.pop_back() ));
                            }
                            ++curr;
                            break;
                            
                        case '+':
                            YRX(std::cerr << "[XP] joker '+'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '+')",fn);
                            else
                            {
                                p->append( one_or_more(p->operands.pop_back() ));
                            }
                            ++curr;
                            break;
                            
                        case '*':
                            YRX(std::cerr << "[XP] joker '*'" << std::endl);
                            if(p->operands.size<=0)
                                throw exception("%s(No Left Expression for '*')",fn);
                            else
                            {
                                p->append( zero_or_more(p->operands.pop_back() ));
                            }
                            ++curr;
                            break;
                            
                            //--------------------------------------------------
                            //
                            // braces
                            //
                            //--------------------------------------------------
                        case LBRACE: {
                            const string content( scan_braces(curr, last) );
                            assert(RBRACE==curr[0]);
                            ++curr;
                            YRX(std::cerr << "[XP] [BRACES] {" << content << "}" << std::endl);
                            if(content.size()<=0)
                                throw exception("%s(Empty Braces)",fn);
                            if(content[0] >= '0' && content[0] <= '9')
                            {
                                //----------------------------------------------
                                // assume counting joker
                                //----------------------------------------------
                            }
                            else
                            {
                                //----------------------------------------------
                                // assume named pattern from dictionary
                                //----------------------------------------------
                                if(!dict)
                                    throw exception("%s(no dict for {%s})", fn, content.c_str());
                                p->append( dict->create(content) );
                            }
                        }
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
                            YRX(std::cerr << "[XP] append single '" << C << "'" << std::endl);
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
                
                p->optimize();
                return pattern::simplify(p.yield());
            }
            
            
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        // high-level
        //
        //
        ////////////////////////////////////////////////////////////////////////
        pattern *compile( const string &expr, const p_dict *dict )
        {
            size_t      depth = 0;
            const char *curr  = expr.c_str();
            const char *last  = curr + expr.size();
            
            return compile_xp(curr,
                              last,
                              dict,
                              depth);
        }
        
        pattern *compile( const char *expr, const p_dict *dict )
        {
            const string xp(expr);
            return compile(xp,dict);
        }
        
    }
}
