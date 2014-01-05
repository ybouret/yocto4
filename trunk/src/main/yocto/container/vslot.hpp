#ifndef YOCTO_VSLOT_INCLUDED
#define YOCTO_VSLOT_INCLUDED 1

#include "yocto/type/traits.hpp"
#include <typeinfo>

namespace yocto
{
    
    //! C-data slot: managing memory
    class cslot
    {
    public:
        cslot() throw();
        ~cslot() throw();
        cslot(size_t n);
        void swap_with( cslot &other ) throw();
        
        void        *data;
        const size_t size;
        
        void prepare(size_t n);
        void release() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(cslot);
    };
    
    //! base class for vslot/xslot
    class basic_slot 
    {
    public:
        explicit basic_slot() throw();
        virtual ~basic_slot() throw();
        explicit basic_slot(size_t);
        
        
        size_t                size() const throw(); //!< allocated bytes
        const std::type_info *info() const throw(); //!< remind me of the type
        const char  *         name() const throw(); //!< from info() or ""
        
        
        //! virtual interface
        virtual bool is_active() const throw() = 0;
        virtual void free()      throw() = 0;
        
        //! Non virtual interface
        void release() throw();
        
        //! status helper
        bool is_allocated() const throw();
        
        //! setup helper
        void prepare_for(size_t n);
        
        //! allocate an INACTIVE slot
        void allocate(size_t n);
        
        //! deallocate an INACTIVE slot
        void deallocate() throw();
        
        
        //! query type comparison
        template <typename T>
        inline bool same_type_than() const throw()
        {
            typedef typename type_traits<T>::mutable_type TT;
            return info_ != 0 && typeid(TT) == *info_;
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline T &as() throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)(slot_.data);
        }
        
        //! transtyping with DEBUG control
        template <typename T>
        inline const T &as() const throw()
        {
            assert(is_active());
            assert(same_type_than<T>());
            return *(T*)(slot_.data);
        }

        
        
    protected:
        cslot                 slot_;
        const std::type_info *info_;           //!< for type id
        void                (*kill_)(void*);   //!< destructor wrapper
        
        template <typename T>
        inline void basic_activate() throw()
        {
            info_ = &typeid(T);
            kill_ = __kill<T>;
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(basic_slot);
        template <typename T>
        static inline
        void __kill( void *addr ) throw()
        {
            assert(addr);
            static_cast<T*>(addr)->~T();
        }

    };
    
    
    //! local dynamic memory, using the object allocator
    class vslot : public basic_slot
    {
    public:
        explicit vslot() throw();
        explicit vslot(size_t n);
        virtual ~vslot() throw();
        
        
        virtual bool is_active() const throw();
        virtual void free() throw();
        
        //! zero argument contructor, automatic memory management
        template <typename T>
        inline void make()
        {
            prepare_for(sizeof(T)); // get memory
            new (slot_.data) T();   // try to construct, may throw
            activate<T>();          // activate the object
        }
        
        //! one argument copy constructor, automatic memory management
        template <typename T>
        inline void make( typename type_traits<T>::parameter_type args )
        {
            prepare_for(sizeof(T));  // get memory
            new (slot_.data) T(args);     // try to construct, may throw
            activate<T>();           // activate the object
        }
        
        //! one argument constructor
        template <typename T,typename U>
        inline void build(typename type_traits<U>::parameter_type args)
        {
            prepare_for(sizeof(T)); // get memory
            new (slot_.data) T(args);    // try to construct, may throw
            activate<T>();          // activate the object
        }
        
        //! two arguments constructor
        template <typename T,typename U, typename V>
        inline void build(typename type_traits<U>::parameter_type u,
                          typename type_traits<V>::parameter_type v )
        {
            prepare_for(sizeof(T));
            new (slot_.data) T(u,v);
            activate<T>();
        }
        
        
        
    private:
        void __free() throw();
        
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
        void activate() throw()
        {
            typedef typename type_traits<T>::mutable_type TT;
            basic_activate<TT>();
        }
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(vslot);
    };
    
}

#endif

