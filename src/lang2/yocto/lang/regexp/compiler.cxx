#include "yocto/lang/regexp/compiler.hxx"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {

        RegExp:: ~RegExp() throw() {}

        RegExp:: RegExp(const char *expr, const size_t n, const p_dict *d) :
        curr(expr),
        last(expr+n),
        dict(d),
        level(0)
        {
            assert(expr!=NULL);
        }

    }
}

namespace yocto
{
    namespace lang
    {

#define LBRACE '{'
#define RBRACE '}'

        static inline
        pattern *extract_tail(p_list &ops, const char type)
        {
            if(ops.size<=0) throw exception("no sub-expression before '%c'", type);
            return ops.pop_back();
        }

        pattern *RegExp:: parse_expr()
        {

            auto_ptr<logical> p(AND::create());

            while(curr<last)
            {
                char C = curr[0];

                switch(C)
                {

                        //______________________________________________________
                        //
                        // PARENS
                        //______________________________________________________
                    case '(':
                        ++curr;  // skip parenthesis
                        ++level; // increase nesting
                        p->append( parse_expr() );
                        break;

                    case ')':
                        ++curr;
                        --level;
                        if(level<0)
                        {
                            throw exception("unexpected end of sub-expression");
                        }
                        goto DONE;

                        //______________________________________________________
                        //
                        // ALTERNATION
                        //______________________________________________________
                    case '|': {
                        ++curr;
                        auto_ptr<pattern> lhs( logical::simplify(p.yield())    );
                        auto_ptr<pattern> rhs( parse_expr() );
                        p.reset( OR::create() );
                        p->append(lhs.yield());
                        p->append(rhs.yield());
                    } goto DONE;


                        //______________________________________________________
                        //
                        // escape sequence
                        //______________________________________________________
                    case '\\':
                        p->append( parse_expr_esc() );
                        break;

                        //______________________________________________________
                        //
                        // single jokers
                        //______________________________________________________
                    case '+':
                        ++curr;
                        p->append( one_or_more(extract_tail(p->operands, '+')));
                        break;

                    case '*':
                        ++curr;
                        p->append( zero_or_more(extract_tail(p->operands, '*')));
                        break;

                    case '?':
                        ++curr;
                        p->append( optional::create(extract_tail(p->operands, '?')));
                        break;

                    case LBRACE:
                        p->append( parse_braces(*p) );
                        break;

                        //______________________________________________________
                        //
                        // default
                        //______________________________________________________
                    default:
                        ++curr;
                        p->append( single::create(C) );
                }

            }

        DONE:
            //______________________________________________________________
            //
            // Consistency check
            //______________________________________________________________
            if(p->operands.size<=0)
            {
                throw exception("empty sub-expression");
            }

            return logical::simplify(p.yield());
        }

    }

}

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lang
    {


        pattern * RegExp:: parse_expr_esc()
        {
            assert(curr<last);
            assert(curr[0]=='\\');
            const char *ini = curr;
            ++curr;
            if(curr>=last) throw exception("unfinished escape sequence");
            const char C = curr[0];
            ++curr;
            switch(C)
            {
                case '\\':
                case '(' :
                case ')' :
                case '[' :
                case ']' :
                case '"' :
                case '\'':
                case '+' :
                case '*' :
                case '?' :
                case '{' :
                case '}' :
                    return single::create(C);

                case 'a': return single::create('\a');
                case 'b': return single::create('\b');
                case 'f': return single::create('\f');
                case 'n': return single::create('\n');
                case 'r': return single::create('\r');
                case 't': return single::create('\t');
                case 'v': return single::create('\v');

                case 'x':
                {
                    if(curr>=last) throw exception("missing first hexadecimal byte in sub-expression");
                    const int B1 = hex2dec(curr[0]);
                    if(B1<0) throw exception("invalid hexadecimal char '%c'", curr[0]);
                    ++curr;
                    if(curr>=last) throw exception("missing second hexadecimal byte in sub-expression");
                    const int B2 = hex2dec(curr[0]);
                    if(B2<0) throw exception("invalid hexademical char '%c' in sub-expression", curr[0]);
                    ++curr;
                    return single::create( B1 * 16 + B2 );
                }

                default:
                    break;

            }
            throw exception("unknown escape sequence '%s'",ini);
        }


    }

}

//#include "yocto/sequence/vector.hpp"
//#include "yocto/memory/pooled.hpp"
//#include "yocto/string/tokenizer.hpp"
#include "yocto/string/conv.hpp"

namespace yocto
{
    namespace lang
    {


        static inline bool whitespaces(char C) throw()
        {
            switch(C)
            {
                case ' ' :
                case '\t':
                case '\r':
                case '\n':
                    return true;
                default: break;
            }
            return false;
        }

        pattern *RegExp:: parse_braces(logical &p)
        {
            assert(LBRACE==curr[0]);
            const char *ini = curr;

            // extract content
            string br;
            while(*(++curr)!=RBRACE)
            {
                if(curr>=last) throw exception("unfinnished braces");
                br.append(curr[0]);
            }
            std::cerr << "CONTENT='" << br << "'" << std::endl;
            ++curr; // skip RBRACE

            // check content
            if(br.size()<=0)
                throw exception("empty braces at '%s'",ini);

            const char ch = br[0];
            if( ch == '_' || (ch>='a' && ch <='z') || (ch>='A' && ch<='Z'))
            {
                // assume substitution from dictionary
                if(!dict) throw exception("not dictionary for '%s'", br.c_str());
                return (*dict)[br];
            }
            else
            {
                // assume n[,m] or something
                char *coma = strchr(br.c_str(), ',');
                if(coma)
                {
                    *coma = 0;
                    string sn(br.c_str());
                    string sm(coma+1);
                    sn.clean(whitespaces);
                    sm.clean(whitespaces);
                    if(sn.size()<=0)
                        throw exception("missing first argument after '%s'", ini);

                    const size_t n = strconv::to_size(sn,"first brace argument");
                    if( sm.size() <= 0 )
                    {
                        return at_least::create(extract_tail(p.operands, LBRACE), n);
                    }
                    else
                    {
                        const size_t m = strconv::to_size(sm,"second brace argument");
                        return counting::create(extract_tail(p.operands, LBRACE), n, m);
                    }

                }
                else
                {
                    br.clean(whitespaces);
                    const size_t n = strconv::to_size(br,"counting argument");
                    return counting::create( extract_tail(p.operands, LBRACE), n, n);
                }
                
            }
            
            
        }
    }
    
}


