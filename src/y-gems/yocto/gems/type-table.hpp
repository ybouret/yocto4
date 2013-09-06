#ifndef YOCTO_GEMS_TYPE_TABLE_INCLUDED
#define YOCTO_GEMS_TYPE_TABLE_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gems
    {
        
        class type_table
        {
        public:
            explicit type_table() throw();
            virtual ~type_table() throw();
            
            void operator()( word_t t, const string &s);
            void operator()( word_t t, const char   *s);
            
            word_t operator[]( const string & ) const;
            word_t operator[]( const char   * ) const;
            const string & operator[](word_t) const;
            
            
            
        private:
            map<word_t,string,key_hasher<word_t,hashing::sfh>,allocator> dmap; //!< direct mapping
            map<string,word_t,key_hasher<string,hashing::sfh>,allocator> rmap; //!< reverse mapping
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(type_table);
            
        };
        
    }
}

#endif

