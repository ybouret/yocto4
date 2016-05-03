
#include "yocto/json/parser.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/tree-walker.hpp"
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

        class Parser :: Impl : public object
        {
        public:
            Value              &value;
            auto_ptr<parser>    prs;
            syntax::tree_walker walker;
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
            }


            inline void call( ios::istream &fp )
            {
                vstk.free();
                auto_ptr<syntax::xnode> tree( prs->parse(fp) );
                //tree->graphviz("json.dot"); ios::graphviz_render("json.dot");
                walker.walk(tree.__get());
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

            inline void on_null(const string&)
            {
                //std::cerr << "[JSON] +null" << std::endl;
                const Value v(IsNull);
                vstk.push(v);
            }

            inline void on_true(const string&)
            {
                //std::cerr << "[JSON] +true" << std::endl;
                const Value v(IsTrue);
                vstk.push(v);
            }

            inline void on_false(const string&)
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

            inline void on_empty_array(const size_t n)
            {
                //std::cerr << "[JSON] +empty_array" << std::endl;
                
                assert(n<=0);
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

            inline void on_empty_object(const size_t n)
            {
                assert(n<=0);
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


#if 0

#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string/conv.hpp"

namespace yocto
{

    using namespace lang;
    
    namespace JSON
    {

        namespace
        {

            static const char json_grammar[] =
            {
#include "./json.inc"
            };

        }


        class Parser :: Impl
        {
        public:
            auto_ptr<parser> P;

            inline Impl() : P( parser::generate("JSON",json_grammar,sizeof(json_grammar)/sizeof(json_grammar[0])))
            {
            }

            inline ~Impl() throw()
            {

            }
            //==================================================================
            // walk the tree
            //==================================================================
            inline void walk(Value               &value,
                             const syntax::xnode *node )
            {
                assert(node!=NULL);
                assert(value.type == IsNull);

                const string &label = node->label;

                if( label == "empty_object" )
                {
                    value.make( IsObject );
                    return;
                }

                if( label == "empty_array" )
                {
                    value.make( IsArray );
                    return;
                }

                if( label == "heavy_array")
                {
                    value.make( IsArray );
                    walk_array( value.as<Array>(), node->children() );
                    return;
                }

                if( label == "heavy_object" )
                {
                    value.make( IsObject );
                    walk_object( value.as<Object>(), node->children() );
                    return;
                }

                if( label == "string" )
                {
                    value.make( IsString );
                    string &str = value.as<String>();
                    const lexeme &lx = node->lex();
                    for( t_char *t = lx.head; t; t=t->next )
                    {
                        const char C = t->code;
                        str.append(C);
                    }

                    return;
                }

                if( label == "number" )
                {
                    value.make( IsNumber );
                    const string str = node->content();
                    value.as<Number>() = strconv::to_real<double>( str, "JSON::Number");
                    return;
                }

                if( label == "null" )
                    return;

                if( label == "true" )
                {
                    value.make( IsTrue );
                    return;
                }

                if( label == "false" )
                {
                    value.make( IsFalse );
                    return;
                }

                throw exception("JSON::walk(Invalid Node <%s>)", label.c_str() );
            }

            inline void walk_array(Array                           &arr,
                                   const syntax::xnode::leaves     &children )
            {
                for( const syntax::xnode *node = children.head; node; node=node->next )
                {
                    { const Value nil; arr.push(nil); }
                    walk( arr[ arr.length()-1 ], node);
                }
            }

            inline void walk_object(Object                          &obj,
                                    const syntax::xnode::leaves     &children )
            {
                for( const syntax::xnode *node = children.head; node; node=node->next )
                {
                    assert( node->label == "pair" );
                    assert( ! node->terminal );
                    const syntax::xnode::leaves &pair = node->children();
                    assert( pair.size == 2);
                    assert(pair.head->label == "string" );
                    assert(pair.head->terminal);
                    const String key = pair.head->content();
                    if( obj.has( key ) )
                        throw exception("JSON: multiple object key '%s', line %u", key.c_str(), unsigned(pair.head->lex().line) );
                    {
                        const Pair   P( key );
                        obj.push(P);
                    }
                    Value &value = obj[ key ];
                    walk(value, pair.tail );
                }
            }

            inline void call( Value &value, ios::istream &in )
            {
                syntax::xnode          *root = P->run(in);
                auto_ptr<syntax::xnode> tree(root);
                if(!tree.is_valid())
                {
                    throw exception("JSON: unexpected empty tree!");
                }
                assert(tree->label=="json");
                assert(tree->size()==1);
                walk(value,root->head());
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
        impl( new Impl() )
        {
        }
        
        Value & Parser:: operator()( ios::istream &in )
        {
            impl->call(value,in);
            return value;
        }
        
        
    }
    
}
#endif

