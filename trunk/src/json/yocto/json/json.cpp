#include "yocto/json/json.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace JSON
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Value
        //
        ////////////////////////////////////////////////////////////////////////
        Value:: Value() throw() :
        type( IsNull ),
        data()
        {
            memset( &data, 0, sizeof(data) );
        }
        
#define YJSON_KILL(TYPE) case Is##TYPE: delete data._##TYPE; break
        
        Value:: ~Value() throw()
        {
            switch( type )
            {
                    
                    YJSON_KILL(String);
                    YJSON_KILL(Array);
                    YJSON_KILL(Object);
                    
                default:
                    break;
            }
            (ValueType&)type = IsNull;
            memset( &data, 0, sizeof(data) );
        }
        
        
        void Value::swap_with( Value &other ) throw()
        {
            cswap<ValueType>( (ValueType&)type, (ValueType&)other.type );
            memswap(&data,&other.data,sizeof(data));
            //core::bswap<sizeof(data)>(data,other.data);
        }
        
        void Value:: nullify() throw()
        {
            Value null;
            swap_with(null);
        }
        
        Value:: Value( const String &s ) :
        type( IsString ),
        data()
        {
            data._String = new String(s);
        }
        
        Value:: Value( const char *s ) :
        type( IsString ),
        data()
        {
            data._String = new String(s);
        }
        
        Value:: Value( const Number x) throw() :
        type( IsNumber ),
        data()
        {
            data._Number = x;
        }
        
        
        Value:: Value( const ValueType of ) :
        type(of),
        data()
        {
            switch( type )
            {
                case IsString: data._String = new String(); break;
                case IsObject: data._Object = new Object(); break;
                case IsArray:  data._Array  = new Array();  break;
                default:
                    break;
            }
        }
        
#define YJSON_COPY(TYPE) case Is##TYPE: data._##TYPE = new TYPE( *(other.data._##TYPE) ); break
        
        Value:: Value( const Value &other ) :
        type( other.type ),
        data()
        {
            switch( type )
            {
                    YJSON_COPY(String);
                    YJSON_COPY(Object);
                    YJSON_COPY(Array);
                    
                default:
                    memcpy( &data, &other.data, sizeof(data) );
                    
            }
        }
        
        Value & Value:: operator=( const Value &other )
        {
            if( this != &other )
            {
                Value tmp(other);
                swap_with(tmp);
            }
            return *this;
        }
        
        
        size_t Value:: length() const throw()
        {
            switch( type )
            {
                case IsArray:  return data._Array->length();
                case IsObject: return data._Object->length();
                default:
                    break;
            }
            return 1;
        }
        
#define YJSON_NAME(TYPE) case Is##TYPE: return #TYPE
        
        const char *Value:: GetTypeName( ValueType t ) throw()
        {
            switch( t )
            {
                    YJSON_NAME(String);
                    YJSON_NAME(Number);
                    YJSON_NAME(Object);
                    YJSON_NAME(Array);
                    YJSON_NAME(True);
                    YJSON_NAME(False);
                    YJSON_NAME(Null);
                default:
                    break;
            }
            return "Unknown";
        }
        
        const char * Value:: type_name() const throw()
        {
            return Value::GetTypeName(type);
        }
        
        Value & Value:: operator[]( size_t index )
        {
            switch( type )
            {
                case IsObject: return (* data._Object)[index];
                case IsArray:  return (* data._Array) [index];
                default:
                    break;
            }
            if( index > 0 )
                throw exception("invalid index>0 for JSON::%s", type_name());
            return *this;
        }
        
        const Value & Value:: operator[]( size_t index ) const
        {
            switch( type )
            {
                case IsObject: return (* data._Object)[index];
                case IsArray:  return (* data._Array) [index];
                default:
                    break;
            }
            if( index > 0 )
                throw exception("invalid index>0 for JSON::%s", type_name());
            return *this;
        }
        
        Value       & Value:: operator[]( const String &id )
        {
            if( type != IsObject ) throw exception("JSON::invalid type %s for ['%s']", type_name(), &id[0] );
            return (* data._Object)[ id ];
        }
        
        const Value       & Value:: operator[]( const String &id ) const
        {
            if( type != IsObject ) throw exception("JSON::invalid const type %s for ['%s']", type_name(), &id[0] );
            return (* data._Object)[ id ];
        }
        
        Value & Value:: operator[]( const char *txt )
        {
            const String id( txt );
            return (*this)[id];
        }
        
        const Value & Value:: operator[]( const char *txt ) const
        {
            const String id( txt );
            return (*this)[id];
        }
        
        
#define YJSON_AS(TYPE) \
TYPE & Value::as##TYPE () {\
if( type != Is##TYPE ) throw exception( "as" #TYPE "(%s)", type_name() ); \
return * data._##TYPE; }\
const TYPE & Value::as##TYPE () const {\
if( type != Is##TYPE ) throw exception( "as" #TYPE "(%s) const", type_name() ); \
return * data._##TYPE; }
        
        YJSON_AS(Array)
        YJSON_AS(String)
        YJSON_AS(Object)
        
        Number       &Value:: asNumber()
        {
            if( type != IsNumber )
                throw exception("asNumber(%s)", type_name());
            return data._Number;
        }
        
        const Number   &Value:: asNumber() const
        {
            if( type != IsNumber )
                throw exception("asNumber(%s) const", type_name());
            return data._Number;
        }
        
        
        Value & Value:: operator=( const String &txt )
        {
            Value tmp(txt);
            swap_with(tmp);
            return *this;
        }
        
        Value & Value:: operator=( const char *txt )
        {
            Value tmp(txt);
            swap_with(tmp);
            return *this;
        }
        
        Value & Value:: operator=( const Number X )
        {
            Value tmp(X);
            swap_with(tmp);
            return *this;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Array
        //
        ////////////////////////////////////////////////////////////////////////
        
        
        Array:: Array() throw() : values() {}
        
        Array:: ~Array() throw() { }
        
        Array:: Array( const Array &other ) : values( other.values )
        {
        }
        
        size_t Array:: length() const throw() { return values.size(); }
        
        Value &Array:: operator[]( size_t index )
        {
            if( index < length() )
                return values[index+1];
            else
            {
                const Value nil;
                while( index < length() )
                {
                    values.push_back( nil );
                }
                return values[index+1];
            }
        }
        
        const Value &Array:: operator[]( size_t index ) const
        {
            if( index >= length() )
                throw exception("const JSON::Array[%u>=%u]", unsigned( index ), unsigned( length() ) );
            return values[index+1];
        }
        
        void Array:: push( const Value &v )
        {
            values.push_back( v );
        }
        
        void Array:: pop()   { if( length() <= 0 ) throw exception("JSON::Array::pop(EMPTY)");   values.pop_back();  }
        void Array:: shift() { if( length() <= 0 ) throw exception("JSON::Array::shift(EMPTY)"); values.pop_front(); }
        void Array:: unshift( const Value &v )
        {
            values.push_front(v);
        }
        
        void Array:: reverse() throw()
        {
            if( length() > 0 )
            {
                mreverse<Value>( &values[1], values.size() );
            }
        }
        
        void Array:: push_swap( Value &v )
        {
            const Value nil;
            values.push_back(nil);
            values.back().swap_with(v);
        }


        
        ////////////////////////////////////////////////////////////////////////
        //
        // Pair
        //
        ////////////////////////////////////////////////////////////////////////
        Pair:: Pair( const string &id ) : name(id), value() {}
        Pair:: Pair( const char   *id ) : name(id), value() {}
        
        Pair:: ~Pair() throw() {}
        const String &Pair:: key() const throw() { return name; }
        
        Pair:: Pair( const Pair &other ) : name( other.name ), value( other.value ) {}
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Object
        //
        ////////////////////////////////////////////////////////////////////////
        Object::  Object() throw() : pairs() {}
        Object:: ~Object() throw() {}
        
        Object:: Object( const Object &other ) : pairs( other.pairs ) {}
        
        size_t Object:: length() const throw() { return pairs.size(); }
        
        void Object:: push( const Pair &p )
        {
            if( ! pairs.insert(p) )
                throw exception("JSON::Object.push(multiple '%s')", p.name.c_str() );
        }
        
        void Object:: pop( const String &key )
        {
            (void) pairs.remove(key);
        }
        
        const Value & Object:: operator[]( const String &id ) const
        {
            const Pair *p = pairs.search( id );
            if( !p ) throw exception("No JSON::Object['%s']", &id[0] );
            return p->value;
        }
        
        const Value & Object:: operator[]( const char *id ) const
        {
            const String _id(id);
            return (*this)[ _id ];
        }
        
        Value & Object:: operator[]( const String &id )
        {
            Pair *p = pairs.search( id );
            if( !p )
            {
                const Pair P( id );
                if( ! pairs.insert( P ) )
                    throw exception("JSON::Object[]: unexpected failure 1/2");
                p = pairs.search( id  );
                if( !p )
                    throw exception("JSON::Object[]: unexpected failure 2/2");
            }
            return p->value;
        }
        
        Value & Object:: operator[]( const char *id )
        {
            const String _id(id);
            return (*this)[_id];
        }
        
        Value & Object:: operator[]( size_t index )
        {
            if( index >= length() ) throw exception("JSON::Object[%u>=%u]", unsigned( index ), unsigned( length() ) );
            return pairs(index+1).value;
        }
        
        const Value & Object:: operator[]( size_t index ) const
        {
            if( index >= length() ) throw exception("const JSON::Object[%u>=%u]", unsigned( index ), unsigned( length() ) );
            return pairs(index+1).value;
        }
        
        bool Object:: has( const String &key ) const throw()
        {
            return pairs.search(key) != NULL;
        }
        
        
        void Value:: make( const ValueType of )
        {
            Value tmp( of ); swap_with(tmp);
        }
        
        
    }
    
}
