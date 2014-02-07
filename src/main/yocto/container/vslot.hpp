#ifndef YOCTO_VSLOT_INCLUDED
#define YOCTO_VSLOT_INCLUDED 1

#include "yocto/container/basic-slot.hpp"

namespace yocto
{
    
    
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
        
        void swap_with(vslot &other) throw();
        
    private:
        void __free() throw();
        
        //______________________________________________________________________
        //
        // activation
        //______________________________________________________________________
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

