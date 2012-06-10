#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/string/conv.hpp"

#define Y_JSON_OUTPUT
#if defined(Y_JSON_OUTPUT)
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>
#endif

namespace yocto 
{
    using namespace lang;
    
    namespace JSON
    {
        
        
        class Parser :: Impl : public object, public lexer, public grammar
        {
        public:
            
            
            lexical::scanner  &scan;
            lexical::scanner  &jstr;
            string             _str;
            
            Impl() : 
            lexer(   "JSON::Lexer" ),
            grammar( "JSON::Parser" ),
            scan( first() ),
            jstr( declare("JSON::String") ),
            _str()
            {
                
                //--------------------------------------------------------------
                //
                // declare JSON terminals
                //
                //--------------------------------------------------------------
                scan.make("LBRACK","\\[" );
                scan.make("RBRACK","\\]" );
                scan.make("LBRACE", "\\{");
                scan.make("RBRACE", "\\}");
                scan.make("COMMA",  ","  );
                scan.make("null",   "null" );
                scan.make("true",   "true" );
                scan.make("false",  "false" );
                scan.make("NUMBER", "[:digit:]+" );
                scan.make("COLUMN", ":");
                scan.call("JSON::String",  "\"", this, &Impl::OnEnterString);
                scan.make( "BLANKS", "[ \t]+", & scan.discard );
                scan.make( "ENDL", "[:endl:]", & scan.newline );
                
                jstr.back("\"", this, &Impl::OnLeaveString);
                jstr.make("1CHAR", ".", this, &Impl::OnChar);
                
                
                //--------------------------------------------------------------
                //
                // declare JSON Grammar
                //
                //--------------------------------------------------------------
                
                //-- main rule
                syntax::alternate & INSTANCE = alt("INSTANCE");
                
                //--------------------------------------------------------------
                // terminals
                //--------------------------------------------------------------
                syntax::terminal &LBRACK = term( "LBRACK", syntax::is_discardable );
                syntax::terminal &RBRACK = term( "RBRACK", syntax::is_discardable );
                syntax::terminal &LBRACE = term( "LBRACE", syntax::is_discardable );
                syntax::terminal &RBRACE = term( "RBRACE", syntax::is_discardable );
                syntax::terminal &COMMA  = term( "COMMA",  syntax::is_discardable );
                syntax::terminal &NUMBER = term( "NUMBER" );
                syntax::terminal &Null   = term( "null",   syntax::is_specialized );
                syntax::terminal &True   = term( "true",   syntax::is_specialized );
                syntax::terminal &False  = term( "false",  syntax::is_specialized );
                syntax::terminal &STRING = term( "JSON::String" );
                syntax::terminal &COLUMN = term( "COLUMN", syntax::is_discardable );
                
                //--------------------------------------------------------------
                // initialize value
                //--------------------------------------------------------------
                syntax::alternate &VALUE = alt("VALUE");
                VALUE << NUMBER << STRING << Null << True << False;
                
                //--------------------------------------------------------------
                // ARRAY
                //--------------------------------------------------------------
                syntax::alternate &ARRAYS = alt( "ARRAYS" );
                
                {
                    {
                        syntax::aggregate &EMPTY_ARRAY = agg( "EMPTY_ARRAY" );
                        EMPTY_ARRAY << LBRACK << RBRACK;
                        ARRAYS |= EMPTY_ARRAY;
                    }
                    {
                        syntax::aggregate &ARRAY       = agg( "ARRAY" );
                        syntax::aggregate &TAIL_VALUE  = agg( "TAIL_VALUE", syntax::is_merging_all );
                        TAIL_VALUE << COMMA << VALUE;
                        
                        ARRAY << LBRACK << VALUE << rep( "ZERO_OR_MORE_TAIL_VALUE", TAIL_VALUE, 0 ) << RBRACK;
                        ARRAYS |= ARRAY;
                    }
                }
                
                VALUE |= ARRAYS;
                
                //--------------------------------------------------------------
                // OBJECT
                //--------------------------------------------------------------
                syntax::alternate &OBJECTS = alt("OBJECTS");
                {
                    {
                        syntax::aggregate &EMPTY_OBJECT = agg( "EMPTY_OBJECT" );
                        EMPTY_OBJECT << LBRACE << RBRACE;
                        OBJECTS |= EMPTY_OBJECT;
                    }
                    {
                        syntax::aggregate &PAIR = agg("PAIR");
                        PAIR << STRING << COLUMN << VALUE;
                        syntax::aggregate &TAIL_PAIR = agg("TAIL_PAIR", syntax::is_merging_all);
                        TAIL_PAIR << COMMA << PAIR;
                        syntax::aggregate &OBJECT = agg("OBJECT");
                        OBJECT << LBRACE << PAIR << rep("ZERO_OR_MORE_TAIL_PAIR", TAIL_PAIR, 0 ) << RBRACE;
                        OBJECTS |= OBJECT;
                    }
                    
                }
                VALUE |= OBJECTS;
                
                INSTANCE |= ARRAYS;
                INSTANCE |= OBJECTS;
                
            }
            
            virtual ~Impl() throw()
            {
                
            }
            
            void OnEnterString( const regex::token & )
            {
                _str.clear();
            }
            
            void OnLeaveString( const regex::token & )
            {
                unget( jstr, _str);
            }
            
            bool OnChar( const regex::token &t )
            {
                for( regex::t_char *ch = t.head; ch; ch=ch->next )
                    _str.append( ch->data );
                
                return false; // not a lexeme !
            }
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, ios::istream &fp )
            {
                value.nullify(); //! make a null value 
                reset();         //! lexer reset
                
                regex::source                S(fp);
                auto_ptr<syntax::parse_node> Tree( accept(*this,S) );
                
                Tree->AST();
                
#if defined (Y_JSON_OUTPUT)
                std::cerr << "Saving tree..." << std::endl;
                {
                    ios::ocstream os("json.dot",false);
                    Tree->graphviz("G", os);
                }
                system( "dot -Tpng json.dot -o json.png" );
#endif
                
                walk( value, Tree.__get() );
            }
            
            
            //==================================================================
            // walk the tree
            //==================================================================
            void walk( Value &value, const syntax::parse_node *node )
            {
                assert(node!=NULL);
                assert(value.type == IsNull);
                
                const string &label = node->label;
                
                if( label == "EMPTY_OBJECT" )
                {
                    value.make( IsObject );
                    return;
                }
                
                if( label == "EMPTY_ARRAY" )
                {
                    value.make( IsArray );
                    return;
                }
                
                if( label == "ARRAY") 
                {
                    value.make( IsArray );
                    walk_array( value.asArray(), node->children() );
                    return;
                }
                
                if( label == "OBJECT" )
                {
                    value.make( IsObject );
                    walk_object( value.asObject(), node->children() );
                    return;
                }
                
                if( label == "JSON::String" )
                {
                    value.make( IsString );
                    string &str = value.asString();
                    const lexeme *lx = node->lex();
                    for( regex::t_char *t = lx->head; t; t=t->next )
                    {
                        const char C = t->data;
                        str.append(C);
                    }
                    
                    return;
                }
                
                if( label == "NUMBER" )
                {
                    value.make( IsNumber );
                    const string str = node->lex()->to_string();
                    value.asNumber() = strconv::to_real<double>( str );
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
            
            void walk_array( Array &arr, const syntax::parse_node::child_list &children )
            {
                for( const syntax::parse_node *node = children.head; node; node=node->next )
                {
                    { const Value nil; arr.push(nil); }
                    walk( arr[ arr.length()-1 ], node);
                }
            }
            
            void walk_object( Object &obj, const syntax::parse_node::child_list &children )
            {
                for( const syntax::parse_node *node = children.head; node; node=node->next )
                {
                    assert( node->label == "PAIR" );
                    assert( ! node->terminal );
                    const syntax::parse_node::child_list &pair = node->children();
                    assert( pair.size == 2);
                    assert(pair.head->label == "JSON::String" );
                    assert(pair.head->terminal);
                    const String key = pair.head->lex()->to_string();
                    if( obj.has( key ) )
                        throw exception("JSON: multiple object key '%s', line %u", key.c_str(), unsigned(pair.head->lex()->line) );
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
