#include "yocto/json/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string/conv.hpp"

//#define Y_JSON_OUTPUT 1
#if defined(Y_JSON_OUTPUT)
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>
#endif

namespace yocto
{
    using namespace lingua;
    
    namespace JSON
    {
        
        
        class Parser :: Impl : public parser
        {
        public:
            source             src;
            lexical::scanner  &jstr;
            string             _str;
            
            Impl() :
            parser( "JSON::Lexer" , "JSON::Parser" ),
            src(),
            jstr( declare("JSON::String") ),
            _str()
            {
                
                
                //--------------------------------------------------------------
                //
                // declare JSON Grammar
                //
                //--------------------------------------------------------------
                
                //-- main rule
                syntax::alternative & INSTANCE = alt("INSTANCE");
                
                //--------------------------------------------------------------
                // terminals
                //--------------------------------------------------------------
                syntax::terminal &LBRACK = jettison( "LBRACK", '[');
                syntax::terminal &RBRACK = jettison( "RBRACK", ']');
                syntax::terminal &LBRACE = jettison( "LBRACE", '{' );
                syntax::terminal &RBRACE = jettison( "RBRACE", '}' );
                syntax::terminal &COMMA  = jettison( "COMMA",  ',' );
                syntax::terminal &NUMBER = terminal( "NUMBER", "-?[1-9][0-9]*([.][0-9]+)?([e|E][-+]?[0-9]+)?" );
                syntax::terminal &Null   = univocal( "null",  "null");
                syntax::terminal &True   = univocal( "true",  "true");
                syntax::terminal &False  = univocal( "false", "false");
                syntax::terminal &STRING = term( "JSON::String" );
                syntax::terminal &COLUMN = jettison( "COLUMN", ':');
                
                
                //--------------------------------------------------------------
                // other main rules
                //--------------------------------------------------------------
                scanner.call("JSON::String",  "\"", this, &Impl::OnEnterString);
                Y_LEX_DISCARD(scanner,"BLANKS","[ \t]+");
                Y_LEX_NO_ENDL(scanner);
                
                //--------------------------------------------------------------
                // JSON:: String
                //--------------------------------------------------------------
                jstr.back("\"", this, &Impl::OnLeaveString);
                jstr.make( "ESC",   "\\x5c.", this, &Impl::OnEscape);
                jstr.make( "1CHAR", ".",      this, &Impl::OnChar);
                
                
                
                //--------------------------------------------------------------
                // initialize value
                //--------------------------------------------------------------
                syntax::alternative &VALUE = alt("VALUE");
                VALUE |= NUMBER;
                VALUE |= STRING;
                VALUE |= Null;
                VALUE |= True;
                VALUE |= False;
                
                //--------------------------------------------------------------
                // ARRAY
                //--------------------------------------------------------------
                syntax::alternative &ARRAYS = alt( "ARRAYS" );
                
                {
                    {
                        syntax::aggregate &EMPTY_ARRAY = agg( "EMPTY_ARRAY" );
                        EMPTY_ARRAY += LBRACK;
                        EMPTY_ARRAY += RBRACK;
                        ARRAYS |= EMPTY_ARRAY;
                    }
                    {
                        syntax::aggregate &ARRAY       = agg( "ARRAY" );
                        syntax::aggregate &TAIL_VALUE  = agg( "TAIL_VALUE", syntax::is_merging_all );
                        TAIL_VALUE += COMMA;
                        TAIL_VALUE += VALUE;
                        
                        ARRAY += LBRACK;
                        ARRAY += VALUE;
                        ARRAY += rep( "ZERO_OR_MORE_TAIL_VALUE", TAIL_VALUE, 0 );
                        ARRAY += RBRACK;
                        
                        ARRAYS |= ARRAY;
                    }
                }
                
                VALUE |= ARRAYS;
                
                //--------------------------------------------------------------
                // OBJECT
                //--------------------------------------------------------------
                syntax::alternative &OBJECTS = alt("OBJECTS");
                {
                    {
                        syntax::aggregate &EMPTY_OBJECT = agg( "EMPTY_OBJECT" );
                        EMPTY_OBJECT += LBRACE;
                        EMPTY_OBJECT += RBRACE;
                        OBJECTS |= EMPTY_OBJECT;
                    }
                    
                    {
                        syntax::aggregate &PAIR = agg("PAIR");
                        PAIR += STRING;
                        PAIR += COLUMN;
                        PAIR += VALUE;
                        syntax::aggregate &TAIL_PAIR = agg("TAIL_PAIR", syntax::is_merging_all);
                        TAIL_PAIR += COMMA;
                        TAIL_PAIR += PAIR;
                        syntax::aggregate &OBJECT = agg("OBJECT");
                        OBJECT += LBRACE;
                        OBJECT += PAIR;
                        OBJECT += rep("ZERO_OR_MORE_TAIL_PAIR", TAIL_PAIR, 0 );
                        OBJECT += RBRACE;
                        
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
            
            inline void OnEnterString( const token & ){ _str.clear();        }
            inline void OnLeaveString( const token & ){ emit( jstr, _str);   }
            
            inline bool OnChar( const token &t )
            {
                for( t_char *ch = t.head; ch; ch=ch->next )
                    _str.append( ch->data );
                
                return false; // not a lexeme !
            }
            
            inline bool OnEscape( const token &t )
            {
                assert(t.size==2);
                char C = t.tail->data;
                switch( C )
                {
                    case '\\':
                    case '/':
                    case '"':
                        break;
                        
                    case 'b': C = '\b'; break;
                    case 'f': C = '\f'; break;
                    case 'n': C = '\n'; break;
                    case 'r': C = '\r'; break;
                    case 't': C = '\t'; break;
                        
                    default:
                        throw exception("%s: line %u: invalid escaped char '%c'", jstr.name.c_str(), unsigned(line), C );
                }
                _str.append( C );
                return false; // not a lexeme
            }
            
            //==================================================================
            // main call
            //==================================================================
            void call( Value &value, lingua::input &in )
            {
                value.nullify(); //! make a null value
                reset();         //! lexer reset
                src.attach(in);  //! source reset
                
                try
                {
                    auto_ptr<syntax::xnode> tree( run(src) );
                    
#if defined (Y_JSON_OUTPUT)
                    std::cerr << "Saving tree..." << std::endl;
                    {
                        tree->graphviz("json.dot");
                    }
                    system( "dot -Tpng json.dot -o json.png" );
#endif
                    
                    src.detach();
                    walk( value, tree.__get() );
                }
                catch(...)
                {
                    src.detach();
                    throw;
                }
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
                    for( t_char *t = lx->head; t; t=t->next )
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
                    value.asNumber() = strconv::to_real<double>( str, "JSON::Number");
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
                    assert( node->label == "PAIR" );
                    assert( ! node->terminal );
                    const syntax::xnode::child_list &pair = node->children();
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
        
        Value & Parser:: operator()( lingua::input &in )
        {
            impl->call(value,in);
            return value;
        }
        
        
    }
    
}
