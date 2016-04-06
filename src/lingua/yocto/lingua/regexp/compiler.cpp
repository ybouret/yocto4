#include "yocto/lingua/regexp/compiler.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

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
                        case '?':
                        case '+':
                        case '*':
                            YRX_OUTPUT(Indent(); std::cerr << "$'" << C << "'" << std::endl);
                            Jokerize(p->operands,C);
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
            return p.yield();
        }
        
        pattern * regexp::compile(const char *expr,const p_dict *dict)
        {
            const string Expr(expr);
            return compile(Expr,dict);
        }
    }
    
}
