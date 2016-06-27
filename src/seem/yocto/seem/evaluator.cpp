#include "yocto/seem/evaluator.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/conv.hpp"

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

                explicit Atom(const int user_code, const void *data, const string &label) :
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


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Atom);
            };
        }

        class Evaluator:: VirtualMachine :
        public object
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
                    } break;

                    case SEEM_AXP:
                    case SEEM_MXP:
                    case SEEM_PXP:
                    case SEEM_FUNC:
                        break;
                        
                    default:
                        throw exception("unhandled terminal '%s'", label.c_str());
                }
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
        parser(true),
        vm( new VirtualMachine( *parser.gram ) )
        {
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

            return 0;
        }


    }

}



