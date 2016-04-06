#include "yocto/lingua/regexp/compiler.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/lingua/pattern/posix.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"

#define YRX_VERBOSE 1

#if YRX_VERBOSE ==1
#include <iostream>
#define YRX_OUTPUT(EXPR) do { EXPR; } while(false)
#else
#define YRX_OUTPUT(EXPR)
#endif

namespace yocto
{

    namespace lingua
    {

        static const char fn[] = "regexp";

        //______________________________________________________________________
        //
        // Class to compile regexp
        //______________________________________________________________________
        class RXCompiler
        {
        public:
            inline ~RXCompiler() throw() {}

            const char   *expr;  //!< for message is necessary
            const char   *curr;  //!< current char
            const char   *last;  //!< first invalid char
            int           depth; //!< nesting level
            const p_dict  dloc;  //!< local dictionary, empty
            const p_dict *dict;  //!< user's or local dict

            inline RXCompiler(const string & _expr,
                              const p_dict * _dict) throw() :
            expr( _expr.c_str() ),
            curr( expr ),
            last( expr + _expr.size() ),
            depth(0),
            dloc(),
            dict( _dict ? _dict : &dloc )
            {

            }

#define LPAREN '('
#define RPAREN ')'
#define ALTERN '|'
#define ESCAPE '\\'
#define LBRACK '['
#define RBRACK ']'
#define LBRACE '{'
#define RBRACE '}'

            //__________________________________________________________________
            //
            //
            // check sub-expression
            //
            //__________________________________________________________________
            inline logical *Check( logical *p )
            {
                auto_ptr<logical> guard(p);
                if(p->operands.size<=0)
                {
                    throw exception("%s: empty sub-expression!",fn);
                }
                return guard.yield();
            }

            //__________________________________________________________________
            //
            //
            // start SubExpr
            //
            //__________________________________________________________________
#if YRX_VERBOSE == 1
            inline void Indent() const
            {
                for(int i=0;i<depth;++i)
                {
                    std::cerr << "  ";
                }
            }
#endif

            inline logical *SubExpr()
            {

                auto_ptr<logical> p( AND::create() );

                while(curr<last)
                {
                    //__________________________________________________________
                    //
                    // current char
                    //__________________________________________________________
                    const char C = curr[0];

                    switch(C)
                    {
                            //__________________________________________________
                            //
                            // grouping
                            //__________________________________________________
                        case LPAREN: {
                            YRX_OUTPUT(Indent(); std::cerr << "<+sub>@" << curr[0] << std::endl);
                            ++depth;             // keep track
                            ++curr;              // skip lparen
                            p->add( SubExpr() ); // recursive call
                        } break;

                        case RPAREN : {
                            if(--depth<0) throw exception("%s: extraneous '%c'", fn, RPAREN);
                            YRX_OUTPUT(Indent(); std::cerr << "<sub/>@" << curr[0] << std::endl);
                            return Check(p.yield());
                        } break;


                            //__________________________________________________
                            //
                            // alternation
                            //__________________________________________________
                        case ALTERN: {
                            YRX_OUTPUT(Indent(); std::cerr << ALTERN << std::endl);
                            if(p->operands.size<=0) throw exception("%s: no left sub-expression before '%c'", fn, ALTERN);
                            auto_ptr<logical> q( OR::create() );
                            ++curr; // skip ALTERN
                            logical *rhs = SubExpr();
                            logical *lhs = p.yield();
                            q->add(lhs);
                            q->add(rhs);
                            return q.yield();
                        } break;



                            //__________________________________________________
                            //
                            // simple jokers
                            //__________________________________________________
                        case '.': p->add( posix::dot() ); break;

                        case '?':
                        case '+':
                        case '*':
                            YRX_OUTPUT(Indent(); std::cerr << "$'" << C << "'" << std::endl);
                            Jokerize(p->operands,C);
                            break;

                            //__________________________________________________
                            //
                            // grouping
                            //__________________________________________________
                        case LBRACK:
                            YRX_OUTPUT(Indent(); std::cerr << "<+grp>" << std::endl);
                            p->add( Grp() );
                            YRX_OUTPUT(Indent(); std::cerr << "<-grp>" << std::endl);
                            break;


                            //__________________________________________________
                            //
                            // Escape sequence
                            //__________________________________________________
                        case ESCAPE:
                            YRX_OUTPUT(Indent(); std::cerr << "ESC" << std::endl);
                            p->add( SubEsc() );
                            break;

                            //__________________________________________________
                            //
                            // default: single char...
                            //__________________________________________________
                        default:
                            YRX_OUTPUT(Indent(); std::cerr << '\'' << C << '\'' << std::endl);
                            p->add( single::create(C) );
                            break;
                    }


                    //__________________________________________________________
                    //
                    // next char
                    //__________________________________________________________
                    ++curr;
                }
                return Check(p.yield());
            }

            //__________________________________________________________________
            //
            //
            // make simple Joker
            //
            //__________________________________________________________________
            inline void Jokerize( p_list &ops, const char C )
            {

                if(ops.size<1) throw exception("%s: no pattern before joker '%c'",fn,C);
                switch(C)
                {
                    case '+':
                        ops.push_back( one_or_more ::create(ops.pop_back()) );
                        return;

                    case '*':
                        ops.push_back( zero_or_more::create(ops.pop_back()) );
                        return;

                    case '?':
                        ops.push_back( optional    ::create(ops.pop_back()) );
                        return;

                    default:
                        break;
                }
                throw exception("%s: unexpected joker '%c'",fn,C);
            }

            //__________________________________________________________________
            //
            //
            // Sub Escape Sequence
            //
            //__________________________________________________________________
            single *SubEsc()
            {
                assert(ESCAPE==curr[0]);
                if(++curr>=last) throw exception("%s: unfinished escaped sequence",fn);
                const char C = curr[0];
                switch(C)
                {
                        // specific encoding
                    case 'n': return single::create('\n');
                    case 'r': return single::create('\r');
                    case 't': return single::create('\t');

                        // special chars
                    case '+':
                    case '*':
                    case '?':
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '\\':
                    case '\'':
                    case '\"':
                    case '{':
                    case '}':
                    case '.':
                        return single::create(C);

                        // hexadecimal
                    case 'x':
                        return SubHex();

                        // unknown
                    default:
                        break;
                }
                throw exception("%s: unknown escaped sequence",fn);
            }

            //__________________________________________________________________
            //
            //
            // Sub Hexadecimal escape sequence
            //
            //__________________________________________________________________
            single *SubHex()
            {
                assert('x'==curr[0]);
                if(++curr>=last)
                {
                    throw exception("%s: missing first char for hexadecimal",fn);
                }

                // get hi byte
                const char hiC = curr[0];
                const int  hi = hex2dec(hiC);
                if(hi<0)
                {
                    throw exception("%s: invalid first hex char '%c'", fn, hiC);
                }

                // get lo byte
                if(++curr>=last)
                {
                    throw exception("%s: missing second char for hexadecimal",fn);
                }
                const char loC = curr[0];
                const int  lo  = hex2dec(loC);
                if(lo<0)
                {
                    throw exception("%s: invalid second hex char '%c'", fn, loC);
                }

                // create single char...
                const int C = hi*16+lo;
                return single::create( uint8_t(C) );
            }

            //__________________________________________________________________
            //
            //
            // group sequence
            //
            //__________________________________________________________________
            pattern *Grp()
            {
                assert(LBRACK==curr[0]);

                //______________________________________________________________
                //
                // let's find choice char
                //______________________________________________________________
                if(++curr>=last)
                {
                    throw exception("%s: unfinished group",fn);
                }
                const char choice = curr[0];

                auto_ptr<logical> p;
                switch(choice)
                {
                    case ':':
                        return Posix();

                    case '^':
                        p.reset( NONE::create() );
                        ++curr;
                        break;

                    default:
                        p.reset( OR::create() );
                        break;
                }

                assert(p.is_valid());

                //______________________________________________________________
                //
                // accumulation
                //______________________________________________________________
                while(curr<last)
                {
                    const char C = curr[0];
                    switch(C)
                    {
                        case RBRACK:
                            return Check(p.yield());

                        case LBRACK:
                            p->add( Grp() );
                            break;

                        case ESCAPE:
                            p->add( GrpEsc() );
                            break;

                        default:
                            p->add( single::create(C) );
                    }


                    ++curr;
                }

                throw exception("%s: unfinished group", fn);
            }

            //__________________________________________________________________
            //
            //
            // find posix
            //
            //__________________________________________________________________
            pattern *Posix()
            {
                assert(':'   ==curr[0]);
                assert(LBRACK==curr[-1]);
                const char *org   = ++curr;
                bool        found = false;
                while(curr<last)
                {
                    if(':'==curr[0]&&RBRACK==curr[1])
                    {
                        found = true;
                        break;
                    }
                    ++curr;
                }
                if(!found)
                {
                    throw exception("%s: unifinished POSIX group",fn);
                }
                const string id(org,curr-org);
                YRX_OUTPUT(Indent(); std::cerr << "=<:" << id << ":>" << std::endl);

                ++curr;

                if(id.size()<=0)
                {
                    throw exception("%s: empty POSIX name",fn);
                }

#define YRX_POSIX(NAME) if( #NAME == id ) return posix:: NAME()

                YRX_POSIX(upper);
                YRX_POSIX(lower);
                YRX_POSIX(alpha);
                YRX_POSIX(digit);
                YRX_POSIX(alnum);
                YRX_POSIX(xdigit);
                YRX_POSIX(blank);
                YRX_POSIX(space);
                YRX_POSIX(punct);

                YRX_POSIX(word);
                YRX_POSIX(endl);
                YRX_POSIX(dot);
                YRX_POSIX(cstring);

                throw exception("%s: unknowm POSIX name '%s'", fn, id.c_str());


            }

            //__________________________________________________________________
            //
            //
            // Group Escape Sequence
            //
            //__________________________________________________________________
            single *GrpEsc()
            {
                assert(ESCAPE==curr[0]);
                if(++curr>=last) throw exception("%s: unfinished escaped sequence",fn);
                const char C = curr[0];
                switch(C)
                {
                        // specific encoding
                    case 'n': return single::create('\n');
                    case 'r': return single::create('\r');
                    case 't': return single::create('\t');

                        // special chars
                    case '^':
                    case '[':
                    case ']':
                    case '\\':
                    case '\'':
                    case '\"':
                        return single::create(C);

                        // hexadecimal
                    case 'x':
                        return SubHex();

                        // unknown
                    default:
                        break;
                }
                throw exception("%s: unknown escaped sequence",fn);
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(RXCompiler);
        };


    }

}


namespace yocto
{
    namespace lingua
    {

        
        pattern * regexp::compile(const string &expr,const p_dict *dict)
        {
            RXCompiler        rxcmp(expr,dict);
            auto_ptr<pattern> p( rxcmp.SubExpr() );
            if( rxcmp.depth != 0 )
            {
                throw exception("%s: unfininished sub-expression",fn);
            }
            return pattern::simplify(p.yield());
        }
        
        pattern * regexp::compile(const char *expr,const p_dict *dict)
        {
            const string Expr(expr);
            return compile(Expr,dict);
        }
    }
    
}
