#ifndef YOCTO_XSLOT_INCLUDED
#define YOCTO_XSLOT_INCLUDED 1

#include "yocto/container/basic-slot.hpp"

namespace yocto
{
    
    //! local dynamic memory, using the object allocator+copy constructor
    class xslot : public basic_slot
    {
    public:
        explicit xslot() throw();
        explicit xslot(size_t n);
        xslot( const xslot &other );
        xslot &operator=( const xslot &);
        
        virtual ~xslot() throw();
        
        
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
            prepare_for(sizeof(T));    // get memory
            new (slot_.data) T(args);  // try to construct, may throw
            activate<T>();             // activate the object
        }
        
        //! one argument constructor
        template <typename T,typename U>
        inline void build(typename type_traits<U>::parameter_type args)
        {
            prepare_for(sizeof(T));   // get memory
            new (slot_.data) T(args); // try to construct, may throw
            activate<T>();            // activate the object
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
        
        void swap_with( xslot &other ) throw();
        
        
    private:
        void __free() throw();
        void (*copy_)( void *dest, const void *addr );
        
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
        
        //______________________________________________________________________
        //
        // extended activation
        //______________________________________________________________________
        template <typename T>
        void activate() throw()
        {
            typedef typename type_traits<T>::mutable_type TT;
            basic_activate<TT>();
            copy_ = __copy<TT>;
        }
        
    };
    
}

#endif

