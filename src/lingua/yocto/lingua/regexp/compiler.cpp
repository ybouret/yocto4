#include "yocto/lingua/regexp/compiler.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

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

            const char   *expr;
            const char   *curr;
            const char   *last;
            int           depth;
            const p_dict  dloc;
            const p_dict *dict;

            inline RXCompiler(const string &Expr, const p_dict *user_dict) throw() :
            expr( Expr.c_str() ),
            curr( expr ),
            last( expr + Expr.size() ),
            depth(0),
            dloc(),
            dict( user_dict ? user_dict : &dloc )
            {

            }


            //__________________________________________________________________
            //
            // start SubExpr
            //__________________________________________________________________
            logical *SubExpr()
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
                            // jokers
                            //__________________________________________________
                        case '?':
                        case '+':
                        case '*':
                            Jokerize(p->operands,C);
                            break;

                            //__________________________________________________
                            //
                            // default: single char...
                            //__________________________________________________
                        default:
                            p->add( single::create(C) );
                            break;
                    }


                    //__________________________________________________________
                    //
                    // next char
                    //__________________________________________________________
                    ++curr;
                }


                return p.yield();
            }

            //__________________________________________________________________
            //
            // make simple Joker
            //__________________________________________________________________
            void Jokerize( p_list &ops, const char C )
            {

                if(ops.size<1) throw exception("%s: no pattern before joker '%c'",fn,C);
                switch(C)
                {
                    case '+':
                        break;

                    case '*':
                        break;

                    case '?':
                        break;

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
            RXCompiler rxcmp(expr,dict);
            return rxcmp.SubExpr();
        }
        
        pattern * regexp::compile(const char *expr,const p_dict *dict)
        {
            const string Expr(expr);
            return compile(Expr,dict);
        }
    }
    
}
