#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string/conv.hpp"

//#define Y_JSON_OUTPUT 1
#if defined(Y_JSON_OUTPUT)
#include <cstdlib>
#endif

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
                //tree->graphviz("json.dot");
                //system( "dot -Tpng json.dot -o json.png" );
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
