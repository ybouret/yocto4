#ifndef YOCTO_ASSOCIATIVE_LABELLED_INCLUDED
#define YOCTO_ASSOCIATIVE_LABELLED_INCLUDED

#include "yocto/string.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/type/key.hpp"

namespace yocto
{
 
    template<
    template <typename,typename,typename,typename> class HOST,
    typename T,
	typename KEY_HASHER = key_hasher<string>,
	typename ALLOCATOR  = memory::global::allocator >
    class labelled : public HOST<string,T,KEY_HASHER,ALLOCATOR>
    {
    public:
        typedef string KEY;
        YOCTO_ASSOCIATIVE_KEY_T;
        typedef HOST<string,T,KEY_HASHER,ALLOCATOR> host_type;
        
        explicit labelled() throw() : host_type() {}
        virtual ~labelled() throw() {}
        explicit labelled( size_t n, const as_capacity_t &opt ) : host_type(n,opt) {}
        
        //! map wrapper
        bool store( const string &key, param_type arg )
        {
            return this->insert(key,arg);
        }
        
        //! map  wrapper
        bool store( const char *id, param_type arg )
        {
            const string key = id;
            return this->insert(key,arg);
        }
        
        //! set wrapper
        bool store( param_type arg )
        {
            return this->insert(arg);
        }
        
        
        type * query( const string &key ) throw() { return this->search(key); }
        type * query( const char  *id ) { const string key = id; return this->search(key); }
        
        const_type *query( const string &key) const throw() { return this->search(key); }
        const_type *query( const char   *id) const { const string key=id; return this->search(key); }
        
        bool erase( const string &key ) throw()
        {
            return this->remove(key);
        }
        
        bool erase( const char *id )
        {
            const string key=id; return this->erase(key);
        }
        
    
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(labelled);
    };
}

#endif
