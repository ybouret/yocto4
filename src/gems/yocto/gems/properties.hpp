#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

#include "yocto/associative/dual-set.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace gems
    {
        
        //! properties, accessible by type or name
        class properties : public object, public counted, public vslot
        {
        public:
            const word_t type;
            const string name;
            
            virtual ~properties() throw();
            explicit properties(word_t t, const string &n);
            
            const word_t & key() const throw();       //!< primary   key: the type
            const string & subkey() const throw();    //!< secondary key: the name
            
            static void check_pointer(const void *addr, const string &id, const char *label);
            static void check_pointer(const void *addr, word_t        id, const char *label);
            
            //! prototype to store properties pointer
            template <typename T>
            class table : public dual_set<word_t, string, typename T::pointer,hashing::sfh,allocator>
            {
            public:
                typedef dual_set<word_t, string,typename T::pointer,hashing::sfh,allocator> base_type;
                explicit table() throw() : base_type() {}
                virtual ~table() throw() {}
                static const char Label[];
                
                inline word_t type_of( const string &n ) const
                {
                    const typename T::pointer *p = this->sub_search(n);
                    properties::check_pointer(p,n,Label);
                    return (*p)->type;
                }
                
                inline const string & name_of( word_t t) const
                {
                    const typename T::pointer *p = this->search(t);
                    properties::check_pointer(p,t,Label);
                    return (*p)->name;
                }
                
                inline T & operator[](word_t t)
                {
                    typename T::pointer *p = this->search(t);
                    properties::check_pointer(p,t,Label);
                    return **p;
                }
                
                inline const T & operator[](word_t t) const
                {
                    const typename T::pointer *p = this->search(t);
                    properties::check_pointer(p,t,Label);
                    return **p;
                }
                
                inline T & operator[](const string &n)
                {
                    typename T::pointer *p = this->sub_search(n);
                    properties::check_pointer(p,n,Label);
                    return **p;
                }
                
                inline const T & operator[](const string &n) const
                {
                    const typename T::pointer *p = this->sub_search(n);
                    properties::check_pointer(p,n,Label);
                    return **p;
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(table);
            };
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };
        
        
        
        
    }
    
}



#endif
