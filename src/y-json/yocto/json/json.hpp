#ifndef YOCTO_JSON_INCLUDED
#define YOCTO_JSON_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        typedef bool   Bool;
        typedef string String;
        typedef double Number;
        
        class Array;
        class Object;
        
        
        enum ValueType
        {
            IsString,
            IsNumber,
            IsObject,
            IsArray,
            IsTrue,
            IsFalse,
            IsNull
        };
        
        class Value
        {
        public:
            ~Value() throw();
            
            Value() throw();   //!< IsNull
            Value( const String & );
            Value( const char   * );
            Value( const Number ) throw();
            Value( const ValueType of );
            
            
            Value( const Value & );
            void swap_with( Value &other ) throw();
            Value & operator=( const Value &other );
            
            const ValueType type;
            
        private:
            union 
            {
                String *_String;
                Number  _Number;
                Object *_Object;
                Array  *_Array;
                void   *null;
            } data;
        };
        
    }
    
}

YOCTO_SUPPORT_BITWISE_OPS(JSON::Value)

namespace yocto
{
    namespace JSON 
    {
        
        class Array
        {
        public:
            explicit Array() throw();
            virtual ~Array() throw();
            Array( const Array & );
            size_t length() const throw();
            
            Value &       operator[]( size_t index );       //!< 0..length-1, or create if necessary
            const Value & operator[]( size_t index ) const; //!< 0..length-1 
            
            void push( const Value & );
            void pop();
            void shift();
            void unshift( const Value & );
            void reverse() throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(Array);
            vector<Value> values;
        };
        
        
        
        class Pair 
        {
        public:
            const String name;
            Value        value;
            
            const String &key() const throw();
            Pair( const Pair &);
            
            Pair( const string & );
            Pair( const char   * );
            ~Pair() throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(Pair);
        };
        
        
        
        class Object 
        {
        public:
            explicit Object() throw();
            virtual ~Object() throw();
            Object( const Object & );
            size_t length() const throw();
            
            void push( const Pair & );
            void pop( const String &key );
            
            const Value & operator[]( const String & ) const;
            const Value & operator[]( const char *   ) const;
            
            Value & operator[]( const String & );
            
        private:
            set<String,Pair> pairs;
            YOCTO_DISABLE_ASSIGN(Object);
        };
        
        
    }
    
}


#endif
