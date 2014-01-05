#ifndef YOCTO_BASIC_SLOT_INCLUDED
#define YOCTO_BASIC_SLOT_INCLUDED 1

#include "yocto/container/cslot.hpp"
#include "yocto/type/traits.hpp"
#include <typeinfo>

namespace yocto
{
    
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
        uint32_t              hash() const throw();
        
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
        
        void basic_swap_with( basic_slot &other ) throw();
        
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
    
}

#endif


