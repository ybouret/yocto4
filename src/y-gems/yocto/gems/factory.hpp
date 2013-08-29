#ifndef YOCTO_GEMS_FACTORY_INCLUDED
#define YOCTO_GEMS_FACTORY_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

#include "yocto/associative/set.hpp"
#include "yocto/associative/map.hpp"

#include "yocto/intrusive-ptr.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    
    namespace  gems
    {
        
        template <typename T>
        class record : public object, public counted, public vslot
        {
        public:
            const string name;
            const uint_t type;
            const T      mass;
            const T      inv_mass;
            
            explicit record(const string &particle_name,
                            const uint_t  particle_type,
                            T             particle_mass );
            virtual ~record() throw();
            
            const string & key() const throw();
            
            typedef intrusive_ptr< string,record<T> > ptr;
            typedef set<string,record::ptr,key_hasher<string,hashing::sfh>,allocator> db;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(record);
        };
        
        template <typename T>
        class factory
        {
        public:
            explicit factory();
            virtual ~factory() throw();
            
            record<T> & declare( const string &name, const uint_t type, const T mass);
            
            record<T> & operator[](const string &name);
            record<T> & operator[](const char   *name);
            
            const record<T> & operator[](const string &name) const;
            const record<T> & operator[](const char   *name) const;

            
        private:
            typedef  map<uint_t,string,key_hasher<uint_t,hashing::sfh>,allocator> type_mapper;
            typename record<T>::db records;
            type_mapper            typeIDs;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(factory);
        };
        
        
        
    }
    
}

#endif
