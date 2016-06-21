
#include "yocto/json/parser.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/stock/stack.hpp"
#include <cstdlib>

namespace yocto
{
    using namespace lingua;

    namespace JSON
    {

        namespace
        {
            class vStack : public stack<Value>
            {
            public:
                inline explicit vStack(size_t n) : stack<Value>(n,as_capacity)
                {
                }

                inline virtual ~vStack() throw()
                {
                }

                inline void push_( Value &content )
                {
                    const Value tmp;
                    push(tmp);
                    peek().swap_with(content);
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vStack);
            };
        }

        static const char json_grammar[] =
        {
#include "./json.inc"
        };

#include "./json.def"
        
        class Parser :: Impl : public object
        {
        public:
            Value              &value;
            auto_ptr<parser>    prs;
            syntax::analyzer    walker;
            vStack              vstk;
            stack<Pair>         pstk;

            inline virtual ~Impl() throw() {}

#define SET_TERM_CB(NAME) walker.on_term(#NAME,this,&Impl::on_##NAME)
#define SET_RULE_CB(NAME) walker.on_rule(#NAME,this,&Impl::on_##NAME)

            inline Impl(Value &val) :
            value(val),
            prs( parser::generate(json_grammar,sizeof(json_grammar)) ),
            walker( *prs ),
            vstk(64),
            pstk(64,as_capacity)
            {
                walker.on_term(this,&Impl::on_term);
                walker.on_rule(this,&Impl::on_rule);

#if 0
                SET_TERM_CB(string);
                SET_TERM_CB(number);
                SET_TERM_CB(null);
                SET_TERM_CB(true);
                SET_TERM_CB(false);

                SET_RULE_CB(pair);
                SET_RULE_CB(empty_array);
                SET_RULE_CB(heavy_array);
                SET_RULE_CB(empty_object);
                SET_RULE_CB(heavy_object);
                SET_RULE_CB(json);
#endif
            }

            inline void on_term(const string &label, const string &content)
            {
                switch(walker.hash_term(label))
                {
                    case JSON_false:   on_false();          break;
                    case JSON_null:    on_null();           break;
                    case JSON_number:  on_number(content);  break;
                    case JSON_string:  on_string(content);  break;
                    case JSON_true:    on_true();           break;
                    default:
                        throw exception("JSON: unknown terminal '%s'",label.c_str());
                        break;
                }
            }

            inline void on_rule(const string &label, const size_t ns)
            {
                switch(walker.hash_rule(label))
                {
                    case JSON_empty_array:  assert(0==ns); on_empty_array();  break;
                    case JSON_empty_object: assert(0==ns); on_empty_object(); break;
                    case JSON_heavy_array:  on_heavy_array(ns);               break;
                    case JSON_heavy_object: on_heavy_object(ns);              break;
                    case JSON_json:         on_json(ns);                      break;
                    case JSON_pair:         on_pair(ns);                      break;
                    default:
                        throw exception("JSON: unknown rule '%s'",label.c_str());
                        break;
                }
            }


            inline void call( ios::istream &fp )
            {
                vstk.free();
                prs->restart();
                auto_ptr<syntax::xnode> tree( prs->parse(fp) );
                walker.walk(tree.__get(),NULL);
            }

            inline void on_string(const string &jstr)
            {
                //std::cerr << "[JSON] +string '" << jstr << "'" << std::endl;
                const Value v = jstr;
                vstk.push(jstr);
            }

            inline void on_number(const string &jnum)
            {
                //std::cerr << "[JSON] +number '" << jnum << "'" << std::endl;
                const Value v = atof(jnum.c_str());
                vstk.push(v);
            }

            inline void on_null()
            {
                //std::cerr << "[JSON] +null" << std::endl;
                const Value v(IsNull);
                vstk.push(v);
            }

            inline void on_true()
            {
                //std::cerr << "[JSON] +true" << std::endl;
                const Value v(IsTrue);
                vstk.push(v);
            }

            inline void on_false()
            {
                //std::cerr << "[JSON] +false" << std::endl;
                const Value v(IsFalse);
                vstk.push(v);
            }
            
            inline void on_pair(const size_t n)
            {
                //std::cerr << "[JSON] +pair(" << n << ")" << std::endl;
                assert(n==2);
                assert(vstk.size()>=2);
                //______________________________________________________________
                //
                // extract content and string
                //______________________________________________________________
                Value       v = vstk.peek(); vstk.pop();
                const Value s = vstk.peek(); vstk.pop();

                //______________________________________________________________
                //
                // create and populate the pair
                //______________________________________________________________
                assert(s.type==IsString);
                Pair p( s.as<String>() );
                
                pstk.push(p);
                pstk.peek().value.swap_with(v);
            }

            inline void on_empty_array()
            {
                Value v(IsArray);
                vstk.push_(v);
            }
            
            inline void on_heavy_array(const size_t n)
            {
                //std::cerr << "[JSON] +heavy_array(" << n << ")" << std::endl;
                assert(n>0);
                assert(vstk.size()>=n);

                //______________________________________________________________
                //
                // create an array
                //______________________________________________________________
                Value  v(IsArray);
                Array &a = v.as<Array>();

                //______________________________________________________________
                //
                // collect from value stack in reverse order
                //______________________________________________________________
                for(size_t i=0;i<n;++i)
                {
                    a.push_( vstk.peek() );
                    vstk.pop();
                }
                a.reverse();

                //______________________________________________________________
                //
                // place on value stack
                //______________________________________________________________
                vstk.push_(v);
            }

            inline void on_empty_object()
            {
                Value v(IsObject);
                vstk.push_(v);
            }

            inline void on_heavy_object(const size_t n)
            {
                assert(n>0);
                assert(pstk.size()>=n);
                //______________________________________________________________
                //
                // create and populate object
                //______________________________________________________________
                Value v(IsObject);

                Object &obj = v.as<Object>();
                const int m=-int(n);
                for(int i=m;i<=-1;++i)
                {
                    Pair &p = pstk[i];
                    {
                        const Pair tmp(p.name);
                        obj.push(tmp);
                    }
                    obj[p.name].swap_with(p.value);
                }

                //______________________________________________________________
                //
                // cleanup stack
                //______________________________________________________________
                for(size_t i=n;i>0;--i)
                {
                    pstk.pop();
                }

                //______________________________________________________________
                //
                // place on value stack
                //______________________________________________________________
                vstk.push_(v);
            }


            inline void on_json(const size_t n)
            {
                assert(1==n);
                assert(1==vstk.size());
                assert(0==pstk.size());
                value.swap_with(vstk.peek());
                vstk.free();
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Impl);
        };



        Parser:: ~Parser() throw()
        {
            delete impl;
        }


        Parser:: Parser() :
        value(),
        impl( new Impl(value) )
        {
        }

        Value & Parser:: operator()( ios::istream &in )
        {
            impl->call(in);
            return value;
        }


    }

}


