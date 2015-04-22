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

        logical *RegExp:: parse_expr()
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
                        auto_ptr<logical> lhs( p.yield()    );
                        auto_ptr<logical> rhs( parse_expr() );
                        p.reset( OR::create() );
                        p->append(lhs.yield());
                        p->append(rhs.yield());
                    } break;

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
            
            return p.yield();
        }
        
    }
    
}
