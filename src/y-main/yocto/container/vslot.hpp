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
        
        bool is_allocated() const throw();
        bool is_active() const throw();
        
    private:
        size_t          size;
        void           *data;
        std::type_info *type;
        void          (*kill)(void*);
        YOCTO_DISABLE_COPY_AND_ASSIGN(vslot);
    };
    
}

#endif

