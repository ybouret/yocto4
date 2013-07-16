#ifndef YOCTO_VSLOT_INCLUDED
#define YOCTO_VSLOT_INCLUDED 1

#include "yocto/type-traits.hpp"
#include <typeinfo>

namespace yocto
{
    
    class vslot
    {
    public:
        explicit vslot() throw();
        explicit vslot(size_t n);
        virtual ~vslot() throw();
        
        void allocate(size_t n);   //!< memory only
        void deallocate() throw(); //!< once object is destructed
        
        void free() throw();    //!< desctruct object, keep memory
        void release() throw(); //!< free and deallocate
        
        bool is_allocated() const throw(); //!< has some memory ?
        bool is_active() const throw();    //!< has some object ?
        
        const std::type_info *info() const throw(); //!< remind me of the type
        
        
        //! zero argument contructor, automatic memory management
        template <typename T>
        inline void make()
        {
            prepare_for(sizeof(T)); //!< get memory
            new (data) T();         //!< try to construct, may throw
            activate<T>();          //!< activate the object
        }
        
        //! one argument constructor, automatic memory management
        template <typename T>
        inline void make( typename type_traits<T>::parameter_type args )
        {
            prepare_for(sizeof(T)); //!< get memory
            new (data) T(args);     //!< try to construct, may throw
            activate<T>();          //!< activate the object

        }
        
        //! query type comparison
        template <typename T>
        bool same_type_than() const throw()
        {
            return type != 0 && typeid(T) == *type;
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline T &as() throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)data;
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline const T &as() const throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)data;
        }
        
        
    private:
        size_t                size;
        void                 *data;
        const std::type_info *type;
        void                (*kill)(void*);
        void prepare_for(size_t n);
        
        template <typename T>
        static inline
        void __kill( void *addr ) throw() { assert(addr); static_cast<T*>(addr)->~T(); }
        
        template <typename T>
        inline void activate() throw()
        {
            kill =  __kill<T>;
            type = &typeid(T);
        }
        YOCTO_DISABLE_COPY_AND_ASSIGN(vslot);
    };
    
}

#endif

