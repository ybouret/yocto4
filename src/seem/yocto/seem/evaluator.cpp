#include "yocto/seem/evaluator.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/conv.hpp"

#include <cmath>

namespace yocto
{
    namespace Seem
    {
#include "seem.def"

        namespace
        {
            class Atom;
            typedef core::list_of_cpp<Atom> Atoms;

            class Atom : public object
            {
            public:
                Atom     *next;
                Atom     *prev;
                const int code;
                union
                {
                    double  number;
                    string *id;
                    Atoms  *args;
                };

                inline virtual ~Atom() throw()
                {
                    switch(code)
                    {
                        case SEEM_ID:
                            delete id;
                            break;

                        case SEEM_ARGS:
                            delete args;
                            break;

                        default:
                            break;
                    }
                }

                inline explicit Atom(const int user_code, const void *data, const string &label) :
                next(0), prev(0), code(user_code)
                {
                    switch(code)
                    {
                        case SEEM_NUMBER:
                            assert(data);
                            number = strconv::to<double>( *(const string *)data, "number" );
                            std::cerr << "push " << label << "=" << number << std::endl;
                            break;

                        case SEEM_ID:
                            assert(data);
                            id     = new string( *(const string *)data );
                            std::cerr << "push " << label << " " << *id << std::endl;
                            break;

                        case SEEM_PLUS:
                        case SEEM_MINUS:
                        case SEEM_MUL:
                        case SEEM_DIV:
                        case SEEM_MOD:
                            std::cerr << "push " << label << std::endl;
                            break;

                        case SEEM_ARGS:
                            args = new Atoms();
                            break;

                        default:
                            break;
                    }
                }

                inline explicit Atom(const double value) throw() :
                next(0), prev(0), code(SEEM_NUMBER)
                {
                    number = value;
                    std::cerr << "push number " << number << std::endl;
                }

                inline friend std::ostream & operator<<( std::ostream &os, const Atom &a )
                {
                    switch(a.code)
                    {
                        case SEEM_NUMBER:
                            os << "<" << a.number << ">";
                            break;

                        case SEEM_ID:
                            os << "'" << *(a.id) << "'";
                            break;

                        case SEEM_PLUS:   os << "PLUS";  break;
                        case SEEM_MINUS:  os << "MINUS"; break;
                        case SEEM_MUL:    os << "MUL";   break;
                        case SEEM_DIV:    os << "DIV";   break;
                        case SEEM_MOD:    os << "MOD";   break;
                            break;

                        case SEEM_ARGS:
                            os << '(';
                            for(const Atom *sub = a.args->head;sub;sub=sub->next)
                            {
                                os << *sub;
                                if(sub->next) os << ',';
                            }
                            os << ')';
                            break;

                        default:
                            break;
                    }

                    return os;
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Atom);
            };
        }

        class Evaluator:: VirtualMachine :  public object
        {
        public:
            lingua::syntax::analyzer engine;
            Atoms                    tstack;
            Atoms                    astack;

            explicit VirtualMachine( const lingua::syntax::grammar &G ) :
            engine(G)
            {
                engine.on_term(this, & VirtualMachine::OnTerm );
                engine.on_rule(this, & VirtualMachine::OnRule );
            }

            virtual ~VirtualMachine() throw()
            {
            }

            inline void OnTerm(const string &label, const string &content)
            {
                const int code = engine.hash_term(label);
                switch(code)
                {
                    case SEEM_NUMBER:
                    case SEEM_ID:
                    case SEEM_PLUS:
                    case SEEM_MINUS:
                    case SEEM_MUL:
                    case SEEM_DIV:
                    case SEEM_MOD:
                        tstack.push_back( new Atom(code,&content,label) );
                        break;

                    default:
                        throw exception("unhandled terminal '%s'", label.c_str());
                }
            }


            inline
            void Fetch( Atoms &ops, const size_t ns ) throw()
            {
                assert(tstack.size>=ns);
                for(size_t i=ns;i>0;--i)
                {
                    ops.push_front( tstack.pop_back() );
                }
            }

            inline void OnRule(const string &label, const size_t ns)
            {
                std::cerr << "call " << label << "/" << ns << std::endl;
                const int code = engine.hash_rule(label);
                switch(code)
                {

                    case SEEM_ARGS: {
                        assert(tstack.size>=ns);
                        astack.push_back( new Atom(SEEM_ARGS,0,label) );
                        Atoms *args = astack.tail->args;
                        for(size_t i=1;i<=ns;++i)
                        {
                            args->push_front( tstack.pop_back() );
                        }
                        std::cerr << "args=" << *(astack.tail) << std::endl;
                    } break;

                    case SEEM_AXP: OnAXP(ns); break;

                    case SEEM_MXP: OnMXP(ns); break;

                    case SEEM_PXP: OnPXP(ns); break;

                    case SEEM_FUNC:
                    {

                    }   break;

                    default:
                        throw exception("unhandled terminal '%s'", label.c_str());
                }
            }

            inline double ToNumber( const Atom *a ) const
            {
                assert(a);
                switch(a->code)
                {
                    case SEEM_NUMBER: return a->number;
                    case SEEM_ID:

                    default:
                        throw exception("Seem: illegal ToNumber call");
                }
            }

            //__________________________________________________________________
            //
            // executing MXP
            //__________________________________________________________________
            inline
            void OnMXP(const size_t ns)
            {
                //std::cerr << "==> MXP/" << ns << std::endl;
                assert( 0 != (1&ns) );
                Atoms ops;
                Fetch(ops,ns);

                const Atom  *a   = ops.head;
                double       ans = ToNumber(a);

                a=a->next;
                while(a)
                {
                    assert(a->next);
                    const int    op  = a->code;     a=a->next;
                    const double rhs = ToNumber(a); a=a->next;
                    switch(op)
                    {
                        case SEEM_MUL: ans *= rhs; break;
                        case SEEM_DIV: ans /= rhs; break;
                        case SEEM_MOD: ans = fmod(ans,rhs); break;
                        default:
                            throw exception("Seem: illegal MXP op");
                    }
                }
                tstack.push_back( new Atom(ans) );
            }

            //__________________________________________________________________
            //
            // executing PXP
            //__________________________________________________________________
            inline
            void OnPXP(const size_t ns)
            {
                //std::cerr << "==> PXP/" << ns << std::endl;
                assert(ns>=2);
                Atoms ops;
                Fetch(ops,ns);

                const Atom  *a   = ops.head;
                double       ans = ToNumber(a);
                for(a=a->next;a;a=a->next)
                {
                    ans = pow( ans, ToNumber(a) );
                }
                tstack.push_back( new Atom(ans) );
            }

            //__________________________________________________________________
            //
            // executing AXP
            //__________________________________________________________________
            inline
            void OnAXP(const size_t ns)
            {
                //std::cerr << "==> AXP/" << ns << std::endl;
                Atoms ops;
                Fetch(ops,ns);

                bool        neg = false;
                const Atom *a   = ops.head;
                switch(a->code)
                {
                    case SEEM_MINUS: neg=true;
                    case SEEM_PLUS:  a=a->next;
                        break;

                    default:
                        break;
                }
                assert(a);
                double ans = ToNumber(a);
                if(neg) { ans=-ans; }

                a=a->next;
                while(a)
                {
                    assert(a->next);
                    const int    op  = a->code;     a=a->next;
                    const double rhs = ToNumber(a); a=a->next;
                    switch(op)
                    {
                        case SEEM_PLUS:  ans += rhs; break;
                        case SEEM_MINUS: ans -= rhs; break;
                        default:
                            throw exception("Seem: illegal AXP op");
                    }
                }

                tstack.push_back( new Atom(ans) );
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(VirtualMachine);
        };
    }
}

namespace yocto
{
    namespace Seem
    {


        Evaluator:: ~Evaluator() throw()
        {
        }


        Evaluator:: Evaluator() :
        parser(false),
        vm( new VirtualMachine( *parser.gram ) )
        {
            std::cerr << "sizeof(Evaluator::VirtualMachine)=" << sizeof(VirtualMachine) << std::endl;
        }

        double Evaluator:: run(ios::istream &fp)
        {
            //__________________________________________________________________
            //
            // parse expresssion
            //__________________________________________________________________
            parser.impl->restart();
            auto_ptr<XNode> tree( parser.impl->parse(fp) );
            
            {
                const string dotname = parser.gram->name + "_output.dot";
                tree->graphviz(dotname);
                ios::graphviz_render(dotname);
            }
            
            //__________________________________________________________________
            //
            // compile: walk...
            //__________________________________________________________________
            std::cerr << "--Walking..." << std::endl;
            vm->engine.walk( tree.__get(), NULL );
            
            std::cerr << "TermStack:" << std::endl;
            for(const Atom *a = vm->tstack.head;a;a=a->next)
            {
                std::cerr << "\t" << *a << std::endl;
            }
            
            return 0;
        }
        
        
    }
    
}



