#include "yocto/lingua/pattern/compiler.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/exception.hpp"

#define YRX_VERBOSE 1

#if defined(YRX_VERBOSE) && (YRX_VERBOSE == 1 )
#define YRX(CODE) do { CODE; } while(false)
#else
#define YRX(CODE)
#endif

namespace yocto
{
    namespace lingua
    {
        
        namespace
        {
            
            pattern *compile_xp(const char   * &curr,
                                const char   * last,
                                const p_dict *dict,
                                size_t       &depth )
            {
                static const char fn[] = "lingua::compile";
                
                auto_ptr<logical> p( AND::create() );
                while(curr<last)
                {
                    char C = *curr;
                    switch(C)
                    {
                            
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
                if(p->operands.size<=0)
                    throw exception("%s(Empty SubExpression)",fn);
                
                p->optimize();
                
                return p.yield();
                //return pattern::simplify(p.yield());
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
