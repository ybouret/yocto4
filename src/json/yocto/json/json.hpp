#ifndef YOCTO_JSON_INCLUDED
#define YOCTO_JSON_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ostream.hpp"

#include <typeinfo>

namespace yocto
{

    namespace JSON
    {
        typedef string String; //!< JSON String
        typedef double Number; //!< JSON Number
        class          Array;  //!< JSON Array
        class          Object; //!< JSON Object


        //! JSON enumerated types.
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


        //! Variant to hold any JSON value
        class Value
        {
        public:
            static const char *GetTypeName( ValueType t ) throw();

            ~Value() throw();

            Value() throw();               //!< IsNull
            Value( const String & );       //!< IsString
            Value( const char   * );       //!< IsString
            Value( const Number ) throw(); //!< IsNumber
            Value( const ValueType of );   //!< type = of, empty type


            Value( const Value & );
            void swap_with( Value &other ) throw();
            Value & operator=( const Value &other );

            Value & operator=( const String & );
            Value & operator=( const char   * );
            Value & operator=( const Number   );



            const char *type_name() const throw();
            size_t length() const throw(); //!< #values in it, default is 1.

            Value & operator[]( size_t index );             //!< Object/Array wrapper
            const Value & operator[]( size_t index ) const; //!< Object/Array wrapper

            Value       & operator[]( const String &);       //!< Object wrapper
            const Value & operator[]( const String &) const; //!< Object wrapper

            Value & operator[]( const char * );              //!< Object wrapper
            const Value & operator[]( const char * ) const;  //!< Object wrapper


            const ValueType type;

            void nullify() throw();
            void make( const ValueType of );

            void output( ios::ostream &fp ) const;
            
            friend std::ostream & operator<<( std::ostream &os, const Value &v);
            
            bool             is_same_type_than( const std::type_info &tid ) const throw();
            static ValueType TypeFor( const std::type_info &tid );

            template <typename T>
            inline T &as()
            {
                return *(T*)address_of( typeid(T) );
            }

            template <typename T>
            inline const T &as() const
            {
                return *(T*)address_of( typeid(T) );
            }

        private:
            void out( ios::ostream &, size_t depth ) const;
            static void out( const Array  &, ios::ostream &, size_t depth );
            static void out( const Object &, ios::ostream &, size_t depth );
            static string j2s( const String &);
            union
            {
                String *_String;
                Number  _Number;
                Object *_Object;
                Array  *_Array;
            } data;
            void *address_of( const std::type_info &tid ) const;

        };

    }

}

YOCTO_SUPPORT_BITWISE_OPS(JSON::Value)

namespace yocto
{
    namespace JSON
    {

        //! an Array with a little JavaScript API
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

            void push( const ValueType of );
            void push_( Value &content ); //!< push null and swap with content

            //! valid for Array/Object/String/Number
            template <typename T>
            inline T & append()
            {
                push( Value::TypeFor( typeid(T) ) );
                Value &v = values.back();
                return v.as<T>();
            }

            friend std::ostream & operator<<( std::ostream &os, const Array &arr);
            
        private:
            YOCTO_DISABLE_ASSIGN(Array);
            vector<Value> values;
            Value & append(const ValueType of);
        };


        //! a name/value Pair for the Object
        class Pair
        {
        public:
            const String name;
            Value        value;

            const String &key() const throw();
            Pair( const Pair &);

            Pair( const string & ); //!< with a Null value
            Pair( const char   * ); //!< with a Null value
            ~Pair() throw();
        
            friend std::ostream & operator<<( std::ostream &os, const Pair &p);

        private:
            YOCTO_DISABLE_ASSIGN(Pair);
        };


        //! JavaScript like Object
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
            Value & operator[]( const char   * );

            Value & operator[]( size_t index );
            const Value & operator[]( size_t index ) const;

            typedef set<String,Pair>           pairs_type;
            typedef pairs_type::iterator       iterator;
            typedef pairs_type::const_iterator const_iterator;

            inline iterator       begin() throw()       { return pairs.begin(); }
            inline iterator       end()   throw()       { return pairs.end();   }
            inline const_iterator begin() const throw() { return pairs.begin(); }
            inline const_iterator end()   const throw() { return pairs.end();   }

            bool has( const String &key ) const throw();
            void insert( const String &key, const ValueType of);
            void insert( const char   *key, const ValueType of);

            //! Array/Object/String/Number
            template <typename T>
            inline T &insert( const String &key )
            {
                insert(key, Value::TypeFor(typeid(T)));
                Pair  &p = pairs.back();
                Value &v = p.value;
                return v.as<T>();
            }

            //! Array/Object/String/Number
            template <typename T>
            inline T &insert( const char *key )
            {
                insert(key, Value::TypeFor(typeid(T)));
                Pair  &p = pairs.back();
                Value &v = p.value;
                return v.as<T>();
            }

            friend std::ostream & operator<<( std::ostream &os, const Object &obj);

        private:
            pairs_type pairs;
            YOCTO_DISABLE_ASSIGN(Object);
        };
        
        
    }
    
}


#endif
