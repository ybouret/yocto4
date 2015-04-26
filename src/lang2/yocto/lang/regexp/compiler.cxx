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

////////////////////////////////////////////////////////////////////////////////
//
// Parsing SubExpression
//
////////////////////////////////////////////////////////////////////////////////
#include "yocto/lang/pattern/posix.hpp"

namespace yocto
{
    namespace lang
    {

#define LBRACE '{'
#define RBRACE '}'

#define LBRACK '['
#define RBRACK ']'


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
                        //  jokers
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

                        case '.':
                        ++curr;
                        p->append( posix::dot() );
                        break;

                        //______________________________________________________
                        //
                        //  class
                        //______________________________________________________
                    case LBRACK:
                        p->append( parse_class() );
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

////////////////////////////////////////////////////////////////////////////////
//
// Parsing SubExpression escape char
//
////////////////////////////////////////////////////////////////////////////////
#include "yocto/code/utils.hpp"
#include <cstring>
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
                case '(' :
                case ')' :
                case '[' :
                case ']' :
                case '\\':
                case '"' :
                case '\'':
                case '+' :
                case '*' :
                case '?' :
                case '{' :
                case '}' :
                case '.' :
                    return single::create(C);

                case 'a': return single::create('\a');
                case 'b': return single::create('\b');
                case 'f': return single::create('\f');
                case 'n': return single::create('\n');
                case 'r': return single::create('\r');
                case 't': return single::create('\t');
                case 'v': return single::create('\v');

                case 'x':
                    return parse_esc_hexa("sub-expression");

                default:
                    break;

            }
            throw exception("unknown escape sequence '%s'",ini);
        }


    }

}

////////////////////////////////////////////////////////////////////////////////
//
// Parsing Braces
//
////////////////////////////////////////////////////////////////////////////////

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

            //__________________________________________________________________
            //
            // extract content
            //__________________________________________________________________
            string br;
            while(*(++curr)!=RBRACE)
            {
                if(curr>=last) throw exception("unfinnished braces");
                br.append(curr[0]);
            }
            ++curr; // skip RBRACE

            //__________________________________________________________________
            //
            // check content
            //__________________________________________________________________
            if(br.size()<=0)
                throw exception("empty braces at '%s'",ini);

            const char ch = br[0];
            if( ch == '_' || (ch>='a' && ch <='z') || (ch>='A' && ch<='Z'))
            {
                //______________________________________________________________
                //
                // assume substitution from dictionary
                //______________________________________________________________
                if(!dict) throw exception("not dictionary for '%s'", br.c_str());
                return (*dict)[br];
            }
            else
            {
                //______________________________________________________________
                //
                // assume n[,m] or something
                //______________________________________________________________
                char *coma = (char *)strchr(br.c_str(), ',');
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

////////////////////////////////////////////////////////////////////////////////
//
// Parsing Classes
//
////////////////////////////////////////////////////////////////////////////////


namespace yocto
{
    namespace lang
    {


        pattern * RegExp:: parse_class()
        {
            assert(curr[0]==LBRACK);
            auto_ptr<logical> p;
            char              C=0;
            //__________________________________________________________________
            //
            // first char analysis
            //__________________________________________________________________
            if(++curr>=last) goto UNFINISHED;


            C = *curr;
            switch(C)
            {
                case '-':
                    p.reset( OR::create() );
                    p->append( single::create('-') );
                    ++curr;
                    break;

                case '^':
                    p.reset( NOT::create() );
                    ++curr;
                    break;

                case ':':
                    return parse_posix();

                default:
                    p.reset( OR::create() );
            }
            assert(p.is_valid());

            //__________________________________________________________________
            //
            // loop over bracket content
            //__________________________________________________________________
            while(curr<last)
            {
                C = *curr;
                switch(C)
                {

                        //______________________________________________________
                        //
                        // are we done ?
                        //______________________________________________________
                    case RBRACK:
                        goto DONE;

                        //______________________________________________________
                        //
                        // recursivity
                        //______________________________________________________
                    case LBRACK:
                        p->append( parse_class() );
                        break;

                        //______________________________________________________
                        //
                        // escape sequence
                        //______________________________________________________
                    case '\\':
                        p->append( parse_class_esc() );
                        break;

                        //______________________________________________________
                        //
                        // building a range
                        //______________________________________________________
                    case '-':
                    {
                        // do we have a previous single char ?
                        if(p->operands.size<=0)                    throw exception("no previous character for range");
                        if(p->operands.tail->uuid != single::UUID) throw exception("previous item is not a char for range");

                        // do we have a next single char ?
                        if(++curr>=last)                           throw exception("unfinished range in class");
                        assert(p->operands.tail->uuid == single::UUID);
                        auto_ptr<pattern> q(p->operands.pop_back()); assert(q->uuid==single::UUID); assert(q->content());

                        const code_t      lower = *(const code_t *)(q->content());
                        code_t            upper = 0;

                        q.reset(0);
                        C  = *curr;
                        switch(C)
                        {
                            case LBRACK:
                            case RBRACK:
                                throw exception("invalid '%c' after range", C);

                            case '\\':
                            {
                                q.reset( parse_class_esc() );
                                assert(q->uuid==single::UUID);
                                upper = *(const code_t *)(q->content());
                            } break;

                            default:
                                ++curr;
                                upper = C;
                        }
                        p->append( range::create(lower, upper) );

                    } break;

                    default:
                        ++curr;
                        p->append( single::create(C) );
                }
            }
            if(curr>=last) goto UNFINISHED;
            
        DONE:
            assert(RBRACK==*curr);
            ++curr;
            return logical::simplify(p.yield());

        UNFINISHED:
            throw exception("unfinished class");
        }


    }

}

////////////////////////////////////////////////////////////////////////////////
//
// Parsing POSIX class
//
////////////////////////////////////////////////////////////////////////////////


namespace yocto
{
    namespace lang
    {

#define Y_RX_POSIX(NAME) if(id==#NAME) return posix::NAME()

        pattern *RegExp:: parse_posix()
        {
            assert(':'==*curr);
            const char *ini = curr;
            while( *(++curr) != ':' )
            {
                if(curr>=last) throw exception("unfinished POSIX name");
            }
            assert(curr<last);
            assert(curr>ini);
            assert(':'==*curr);
            const string id(ini+1,(curr-ini)-1);
            ++curr;
            if(curr>=last) throw exception("unfinished POSIX format");
            if(RBRACK!=*curr)
                throw exception("expecting RBRACK after POSIX name");
            ++curr;

            Y_RX_POSIX(lower);
            Y_RX_POSIX(upper);
            Y_RX_POSIX(digit);
            Y_RX_POSIX(alpha);
            Y_RX_POSIX(alnum);
            Y_RX_POSIX(xdigit);
            Y_RX_POSIX(blank);
            Y_RX_POSIX(space);
            Y_RX_POSIX(punct);

            Y_RX_POSIX(word);
            Y_RX_POSIX(endl);
            Y_RX_POSIX(dot);
            Y_RX_POSIX(cstring);

            throw exception("unknown POSIX '%s'", id.c_str());
        }
    }

}
////////////////////////////////////////////////////////////////////////////////
//
// Parsing Class Escape sequence
//
////////////////////////////////////////////////////////////////////////////////

#include "yocto/lang/pattern/posix.hpp"

namespace yocto
{
    namespace lang
    {

        pattern * RegExp:: parse_class_esc()
        {
            assert('\\'==*curr);
            if(++curr>=last) throw exception("unfinished class escape sequence");
            const char C=curr[0];
            ++curr;
            switch(C)
            {
                case '[':
                case ']':
                case '-':
                case '^':
                case '\\':
                case '"' :
                case '\'':
                    return single::create(C);

                case 'a': return single::create('\a');
                case 'b': return single::create('\b');
                case 'f': return single::create('\f');
                case 'n': return single::create('\n');
                case 'r': return single::create('\r');
                case 't': return single::create('\t');
                case 'v': return single::create('\v');
                    
                case 'x':
                    return parse_esc_hexa("class");

                default:
                    break;
            };
            throw exception("unknown class escape sequence '%c'", C);
        }
    }
    
}

////////////////////////////////////////////////////////////////////////////////
//
// Parsing an hexa decimal escape sequence
//
////////////////////////////////////////////////////////////////////////////////
namespace yocto
{
    namespace lang
    {
        pattern * RegExp:: parse_esc_hexa(const char *where)
        {
            assert(where!=0);
            if(curr>=last) throw exception("missing first hexadecimal byte in %s",where);
            const int B1 = hex2dec(curr[0]);
            if(B1<0) throw exception("invalid hexadecimal char '%c' in %s", curr[0], where);
            ++curr;
            if(curr>=last) throw exception("missing second hexadecimal byte in %s",where);
            const int B2 = hex2dec(curr[0]);
            if(B2<0) throw exception("invalid hexademical char '%c' in %s", curr[0], where);
            ++curr;
            return single::create( 16*B1 + B2 );
        }

    }

}
