#ifndef YOCTO_VSLOT_INCLUDED
#define YOCTO_VSLOT_INCLUDED 1

#include "yocto/type/traits.hpp"
#include <typeinfo>

namespace yocto
{
    
    //! local dynamic memory, using the object allocator
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
        
        size_t bytes() const throw();               //!< current allocated bytes
        const std::type_info *info() const throw(); //!< remind me of the type
        
        
        //! zero argument contructor, automatic memory management
        template <typename T>
        inline void make()
        {
            prepare_for(sizeof(T)); // get memory
            new (data_) T();        // try to construct, may throw
            activate<T>();          // activate the object
        }
        
        //! one argument copy constructor, automatic memory management
        template <typename T>
        inline void make( typename type_traits<T>::parameter_type args )
        {
            prepare_for(sizeof(T));  // get memory
            new (data_) T(args);     // try to construct, may throw
            activate<T>();           // activate the object
        }
        
        //! one argument constructor
        template <typename T,typename U>
        inline void build(typename type_traits<U>::parameter_type args)
        {
            prepare_for(sizeof(T)); // get memory
            new (data_) T(args);    // try to construct, may throw
            activate<T>();          // activate the object
        }
        
        //! two arguments constructor
        template <typename T,typename U, typename V>
        inline void build(typename type_traits<U>::parameter_type u,
                          typename type_traits<V>::parameter_type v )
        {
            prepare_for(sizeof(T));
            new (data_) T(u,v);
            activate<T>();
        }
        
        
        //! query type comparison
        template <typename T>
        bool same_type_than() const throw()
        {
            typedef typename type_traits<T>::mutable_type TT;
            return type_ != 0 && typeid(TT) == *type_;
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline T &as() throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)data_;
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline const T &as() const throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)data_;
        }
        
        //! type name of "" if no type
        const char *name() const throw();
        
        
    private:
        size_t                size_;                      //!< allocated bytes for data
        void                 *data_;                      //!< allocated memory area
        const std::type_info *type_;                      //!< for type id
        void                (*kill_)(void*);              //!< destructor wrapper
        //void                (*copy_)(void*,const void *); //!< copy wrapper
        void prepare_for(size_t n);
        
        //______________________________________________________________________
        //
        // destructor wrapper
        //______________________________________________________________________
        template <typename T>
        static inline
        void __kill( void *addr ) throw()
        {
            assert(addr);
            static_cast<T*>(addr)->~T();
        }
        /*
         //______________________________________________________________________
         //
         // copy constructor wrapper
         //______________________________________________________________________
         template <typename T>
         static inline
         void __copy( void *dest, const void *addr )
         {
         assert(dest);
         assert(addr);
         new (dest) T( *(T*)addr );
         }
         */
        
        template <typename T>
        inline void activate() throw()
        {
            typedef typename type_traits<T>::mutable_type TT;
            kill_ =  __kill<TT>;
            //copy_ =  __copy<TT>;
            type_ = &typeid(TT);
        }
        YOCTO_DISABLE_COPY_AND_ASSIGN(vslot);
    };
    
}

#endif

