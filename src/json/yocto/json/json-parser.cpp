#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
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
        
        
        class Parser :: Impl : public parser
        {
        public:
            
            Impl() :
            parser( "JSON", "instance")
            {
                //______________________________________________________________
                //
                //
                // JSON Grammar
                //
                //______________________________________________________________
             
                //______________________________________________________________
                //
                // declare terminals
                //______________________________________________________________
                Terminal &LBRACK = jettison("LBRACK", '[');
                Terminal &RBRACK = jettison("RBRACK", ']');
                Terminal &COMMA  = jettison("COMMA",  ',');
                Terminal &COLUMN = jettison("COLUMN", ':');
                Terminal &LBRACE = jettison("LBRACE", '{');
                Terminal &RBRACE = jettison("RBRACE", '}');
                Rule     &STRING = plug_term(new lexical::cstring("string",*this));
                
                //______________________________________________________________
                //
                // initialize value
                //______________________________________________________________
                Alternate &value = alt();
                value |= STRING;
                value |= univocal("null",  "null");
                value |= univocal("false", "false");
                value |= univocal("true",  "true");
                const char number_expr[] = "-?(0|([1-9][:digit:]*))(\\.[:digit:]*([eE]-?[:digit:]+)?)?";
                pattern   *number_code   = compile(number_expr,NULL);
                value |= terminal("number",number_code);
                
                
                //______________________________________________________________
                //
                // arrays
                //______________________________________________________________
                Alternate &arrays = alt();
                {
                    {
                        Aggregate &empty_array = assemble("empty_array");
                        empty_array << LBRACK << RBRACK;
                        arrays |= empty_array;
                    }
                    
                    {
                        Aggregate &array       = assemble("array");
                        array << LBRACK << value;
                        {
                            Aggregate &extra_value  = merge();
                            extra_value << COMMA << value;
                            array << zero_or_more(extra_value);
                        }
                        
                        array << RBRACK;
                        
                        arrays |= array;
                    }
                }
                value |= arrays;
                
                //______________________________________________________________
                //
                // object
                //______________________________________________________________
                
                Alternate &objects = alt();
                {
                    Aggregate &empty_object = assemble("empty_object",LBRACE,RBRACE);
                    objects |= empty_object;
                }
                
                {
                    Aggregate &heavy_object = assemble("object");
                    heavy_object << LBRACE;
                    {
                        Aggregate &Pair      = assemble("pair",STRING,COLUMN,value);
                        Aggregate &ExtraPair = merge(COMMA,Pair);
                        heavy_object << Pair << zero_or_more(ExtraPair);
                    }
                    heavy_object << RBRACE;
                    objects |= heavy_object;
                }
                value |= objects;
                
                set_root( choose(objects,arrays) );
                
                //______________________________________________________________
                //
                // final
                //______________________________________________________________
                scanner.make("BLANK", "[:blank:]", discard);
                scanner.make("ENDL",  "[:endl:]",  newline);
                
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, ios::istream &in )
            {
                value.nullify(); //! make a null value
                initialize();
                
                
                auto_ptr<syntax::xnode> tree( run(in) );
                
#if defined (Y_JSON_OUTPUT)
                std::cerr << "Saving tree..." << std::endl;
                {
                    tree->graphviz("json.dot");
                }
                system( "dot -Tpng json.dot -o json.png" );
#endif
                
                walk( value, tree.__get() );
                
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
                
                if( label == "array")
                {
                    value.make( IsArray );
                    walk_array( value.as<Array>(), node->children() );
                    return;
                }
                
                if( label == "object" )
                {
                    value.make( IsObject );
                    walk_object( value.as<Object>(), node->children() );
                    return;
                }
                
                if( label == "string" )
                {
                    value.make( IsString );
                    string &str = value.as<String>();
                    const lexeme *lx = node->lxm();
                    for( t_char *t = lx->head; t; t=t->next )
                    {
                        const char C = t->code;
                        str.append(C);
                    }
                    
                    return;
                }
                
                if( label == "number" )
                {
                    value.make( IsNumber );
                    const string str = node->lxm()->to_string();
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
                                   const syntax::xnode::child_list &children )
            {
                for( const syntax::xnode *node = children.head; node; node=node->next )
                {
                    { const Value nil; arr.push(nil); }
                    walk( arr[ arr.length()-1 ], node);
                }
            }
            
            inline void walk_object(Object                          &obj,
                                    const syntax::xnode::child_list &children )
            {
                for( const syntax::xnode *node = children.head; node; node=node->next )
                {
                    assert( node->label == "pair" );
                    assert( ! node->terminal );
                    const syntax::xnode::child_list &pair = node->children();
                    assert( pair.size == 2);
                    assert(pair.head->label == "string" );
                    assert(pair.head->terminal);
                    const String key = pair.head->lxm()->to_string();
                    if( obj.has( key ) )
                        throw exception("JSON: multiple object key '%s', line %u", key.c_str(), unsigned(pair.head->lxm()->line) );
                    {
                        const Pair   P( key );
                        obj.push(P);
                    }
                    Value &value = obj[ key ];
                    walk(value, pair.tail );
                }
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
