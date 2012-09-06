#ifndef YOCTO_SPADE_ARRAY_DB_INCLUDED
#define YOCTO_SPADE_ARRAY_DB_INCLUDED 1

#include "yocto/spade/varray.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace spade
    {
        
        class array_db
        {
        public:
            explicit array_db() throw();
            explicit array_db(size_t);
            virtual ~array_db() throw();
            
            void insert( const varray::ptr &arr );
            
            varray & operator[]( const string & );
            varray & operator[]( const char  *  );
            const varray & operator[]( const string & ) const;
            const varray & operator[]( const char  *  ) const;
            
            
        private:
            set<string,varray::ptr> arrays;
        };
        
    }
}


#endif
