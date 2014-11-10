#ifndef YOCTO_LANG_TYPES_INCLUDED
#define YOCTO_LANG_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lang
    {
        
        //! dynamic char content
        typedef uint8_t  code_type;
        
        //! a store of 0..255 bytes
        typedef
        sorted_vector<
        code_type,
        comparator<code_type>,
        memory::global::allocator> bytes_store;
        
        //! pattern first possible chars
        class first_chars : public bytes_store
        {
        public:
            bool accept_empty;                       //!< if an empty pattern is ok
            explicit first_chars() throw();          //!< default ctor
            virtual ~first_chars() throw();          //!< default dtor
            first_chars(const first_chars &);        //!< copy all
            void add(code_type);                     //!< add one code
            void add(const first_chars&fc);          //!< leave accept_empty untouched
            void sub(code_type) throw();             //!< remove one code
            void sub(const first_chars &fc) throw(); //!< leave accept_empty untouched
            
        private:
            YOCTO_DISABLE_ASSIGN(first_chars);
        };
        
        
    }
}

#endif
