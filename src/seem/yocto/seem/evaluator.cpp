#include "yocto/seem/evaluator.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/associative/htree.hpp"
#include "yocto/container/cslot.hpp"
#include "yocto/sequence/lw-array.hpp"

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

            //! variant class for a dynamic stack
            class Atom : public object
            {
            public:
                Atom     *next; //!< for Atoms
                Atom     *prev; //!< for Atoms
                const int code; //!< one of the SEEM_
                union
                {
                    double  number;
                    string *id;
                    Atoms  *args;
                };

                //! default destructor
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

                //! transform a terminal into an atom
                inline explicit Atom(const int     user_code,
                                     const void   *data,
                                     const string &label) :
                next(0),
                prev(0),
                code(user_code)
                {
                    switch(code)
                    {
                        case SEEM_NUMBER:
                            assert(data);
                            number = strconv::to<double>( *(const string *)data, "number" );
                            break;

                        case SEEM_ID:
                            assert(data);
                            id     = new string( *(const string *)data );
                            break;

                        case SEEM_PLUS:
                        case SEEM_MINUS:
                        case SEEM_MUL:
                        case SEEM_DIV:
                        case SEEM_MOD:
                            break;

                        case SEEM_ARGS:
                            args = new Atoms();
                            break;

                        default:
                            break;
                    }
                }

                //! Transform a real into an atom
                inline explicit Atom(const double value) throw() :
                next(0),
                prev(0),
                code(SEEM_NUMBER)
                {
                    number = value;
                }

#if 0
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
#endif

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Atom);
            };
        }

        //! stack based virtual machine
        class Evaluator:: VirtualMachine :  public object
        {
        public:
            htree<double>            variables;
            htree<Function>          functions;
            lingua::syntax::analyzer engine;
            Atoms                    tstack;

            //! constructor: prepare syntax analyzer
            explicit VirtualMachine( const lingua::syntax::grammar &G ) :
            variables(),
            functions(),
            engine(G),
            tstack()
            {
                engine.on_term(this, & VirtualMachine::OnTerm );
                engine.on_rule(this, & VirtualMachine::OnRule );
            }

            //! default destructor
            virtual ~VirtualMachine() throw()
            {
            }

            //! register a variable value
            void SetVariable(const string &name, const double value)
            {
                double *pvar = variables.find(name);
                if(pvar)
                {
                    *pvar = value;
                }
                else
                {
                    if(!variables.insert(name,value))
                    {
                        throw exception("Seem: unexpected failure to SetVariable(%s,%g)", name.c_str(),value);
                    }
                    variables.optimize();
                }

            }

            //! register a function name
            void SetFunction(const string &name, const Function &fn )
            {
                Function *pfn = functions.find(name);
                if(pfn)
                {
                    *pfn  = fn;
                }
                else
                {
                    if( !functions.insert(name,fn) )
                    {
                        throw exception("Seem: unexpected failure to SetFunction(%s)", name.c_str());
                    }
                    functions.optimize();
                }
            }

            //! push term on stack
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
                        throw exception("Seem: unhandled terminal '%s'", label.c_str());
                }
            }


            //! extract global to local stack, reverse order
            inline
            void Fetch( Atoms &ops, const size_t ns ) throw()
            {
                assert(tstack.size>=ns);
                for(size_t i=ns;i>0;--i)
                {
                    ops.push_front( tstack.pop_back() );
                }
            }

            //! transform stack according to internal
            inline void OnRule(const string &label, const size_t ns)
            {
                const int code = engine.hash_rule(label);
                switch(code)
                {

                    case SEEM_ARGS: {
                        assert(tstack.size>=ns);
                        auto_ptr<Atom> pArgs( new Atom(SEEM_ARGS,0,label) );
                        Fetch( *(pArgs->args), ns );
                        tstack.push_back( pArgs.yield() );
                    } break;

                    case SEEM_AXP:  OnAXP(ns);  break;
                    case SEEM_MXP:  OnMXP(ns);  break;
                    case SEEM_PXP:  OnPXP(ns);  break;
                    case SEEM_FUNC: OnFUNC(ns); break;

                    default:
                        throw exception("Seem: unhandled internal '%s'", label.c_str());
                }
            }


            //! get numeric value from number or variable
            inline double ToNumber( const Atom *a ) const
            {
                assert(a);
                switch(a->code)
                {
                    case SEEM_NUMBER: return a->number;
                    case SEEM_ID:
                    {
                        const string  name = *(a->id);
                        const double *pvar = variables.find(name);
                        if(!pvar) throw exception("Seem: ToNumber(no '%s')", name.c_str());
                        return *pvar;
                    }

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

            //__________________________________________________________________
            //
            // calling func
            //__________________________________________________________________
            inline void OnFUNC(const size_t ns)
            {
                assert(2==ns);
                //______________________________________________________________
                //
                // make a local stack
                //______________________________________________________________

                Atoms ctx;
                Fetch(ctx,2);

                //______________________________________________________________
                //
                // looking for function
                //______________________________________________________________
                assert(SEEM_ID==ctx.head->code);
                const string &name = *(ctx.head->id);
                Function     *pfn  = functions.find(name);
                if(!pfn)
                {
                    throw exception("Seem: unknown function '%s'", name.c_str());
                }

                //______________________________________________________________
                //
                // building args
                //______________________________________________________________
                assert(SEEM_ARGS==ctx.tail->code);
                const Atoms     &args = *(ctx.tail->args);
                const size_t     na   = args.size;
                cslot            slot( na * sizeof(double) );
                double          *addr = static_cast<double *>(slot.data);
                lw_array<double> params(addr,na);
                for(const Atom *a = args.head;a;a=a->next)
                {
                    *(addr++) = ToNumber(a);
                }

                //______________________________________________________________
                //
                // push result on stack
                //______________________________________________________________
                tstack.push_back( new Atom( (*pfn)(params) ) );
            }


            //get a tree
            inline double evaluate( const XNode *tree )
            {
                tstack.clear();
                engine.walk(tree,NULL);
                if(tstack.size!=1)
                {
                    throw exception("Seem: stack too large");
                }
                const double ans = ToNumber(tstack.tail);
                tstack.clear();
                return ans;
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
            //__________________________________________________________________
            // declare some function
            //__________________________________________________________________
#define Y_SEEM_DECL(NAME) SetCFunction(#NAME,NAME)
            Y_SEEM_DECL(cos);
            Y_SEEM_DECL(sin);
            Y_SEEM_DECL(tan);
            Y_SEEM_DECL(acos);
            Y_SEEM_DECL(asin);
            Y_SEEM_DECL(atan);

            Y_SEEM_DECL(cosh);
            Y_SEEM_DECL(sinh);
            Y_SEEM_DECL(tanh);

            Y_SEEM_DECL(exp);
            Y_SEEM_DECL(log);
            Y_SEEM_DECL(log10);

            Y_SEEM_DECL(sqrt);

            Y_SEEM_DECL(ceil);
            Y_SEEM_DECL(floor);
            Y_SEEM_DECL(fabs);

#define Y_SEEM_DECL2(NAME) SetCFunction2(#NAME,NAME)

            Y_SEEM_DECL2(atan2);
            Y_SEEM_DECL2(pow);

        }

        double Evaluator:: run(ios::istream &fp)
        {
            //__________________________________________________________________
            //
            // parse expresssion
            //__________________________________________________________________
            parser.impl->restart();
            auto_ptr<XNode> tree( parser.impl->parse(fp) );

#if 0
            {
                const string dotname = parser.gram->name + "_output.dot";
                tree->graphviz(dotname);
                ios::graphviz_render(dotname);
            }
#endif
            
            //__________________________________________________________________
            //
            // compile: walk...
            //__________________________________________________________________
            const double ans = vm->evaluate( tree.__get() );
            

            
            return ans;
        }


        void Evaluator:: SetVariable(const string &name, const double value)
        {
            vm->SetVariable(name,value);
        }

        void Evaluator:: SetVariable(const char *name, const double value)
        {
            const string NAME(name);
            vm->SetVariable(NAME,value);
        }

        void Evaluator:: SetFunction(const string &name, const Function &fn)
        {
            vm->SetFunction(name,fn);
        }

        void Evaluator:: SetFunction(const char *name, const Function &fn)
        {
            const string NAME(name);
            vm->SetFunction(NAME,fn);
        }

        namespace
        {

            class __CFunction
            {
            public:
                Evaluator::CFunction proc;
                inline  __CFunction(const Evaluator::CFunction fn) throw() : proc(fn) { assert(proc); }
                inline ~__CFunction() throw() {}
                inline  __CFunction(const __CFunction &other) throw() : proc(other.proc) {}

                inline double operator()(const array<double> &arr)
                {
                    assert(1==arr.size());
                    assert(proc);
                    return proc(arr[1]);
                }

            private:
                YOCTO_DISABLE_ASSIGN(__CFunction);
            };
        }

        void Evaluator:: SetCFunction(const char *name, CFunction proc)
        {
            const __CFunction wrapper(proc);
            SetFunction(name,wrapper);
        }

        namespace
        {
            class __CFunction2
            {
            public:
                Evaluator::CFunction2 proc;
                inline  __CFunction2(const Evaluator::CFunction2 fn) throw() : proc(fn) { assert(proc); }
                inline ~__CFunction2() throw() {}
                inline  __CFunction2(const __CFunction2 &other) throw() : proc(other.proc) {}

                inline double operator()(const array<double> &arr)
                {
                    assert(2==arr.size());
                    assert(proc);
                    return proc(arr[1],arr[2]);
                }

            private:
                YOCTO_DISABLE_ASSIGN(__CFunction);
            };

        }


        void Evaluator:: SetCFunction2(const char *name, CFunction2 proc )
        {
            const __CFunction2 wrapper(proc);
            SetFunction(name,wrapper);
        }


    }

}

#include "yocto/ios/imstream.hpp"

namespace yocto
{
    namespace Seem
    {
        double Evaluator:: run(const string  &s)
        {
            ios::imstream fp(s);
            return run(fp);
        }

        double Evaluator:: run(const char *s)
        {
            ios::imstream fp( s, length_of(s) );
            return run(fp);
        }
    }
}

