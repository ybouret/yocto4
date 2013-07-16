#ifndef YOCTO_CHEMICAL_COLLECTION
#define YOCTO_CHEMICAL_COLLECTION 1


#include "yocto/chemical/species.hpp"

namespace yocto
{
    namespace chemical
    {
    
        class collection : public species::db
        {
        public:
            explicit collection() throw();
            explicit collection(size_t);
            virtual ~collection() throw();
            
            species &add( const string &name, int z);
            
            species::ptr &operator[]( const string &id );
            species::ptr &operator[]( const char   *id );
            
            const species::ptr & operator[]( const string &id ) const;
            const species::ptr &operator[]( const char    *id ) const;

            void suppress( const string &name ) throw();
            
            void update_indices() throw(); // in case a species is removed
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(collection);
            
        };
        
    }
    
}

#endif

