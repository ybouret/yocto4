#include "yocto/fovea/linear.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        void   linear_space:: clear() throw()
        {
            (size_t&)bytes = 0;
            (size_t&)items = 0;
        }
        
        linear_space:: ~linear_space() throw()
        {
            memory::kind<memory::global>::release(entry, allocated);
            clear();
        }
        
        static inline
        size_t __check_linear_items( const size_t items )
        {
            if(items<=0)
                throw exception("fovea::linear_space(invalid #items)");
            return items;
        }
        
        static inline
        size_t __check_linear_sizeof( const size_t sz )
        {
            if(sz<=0)
                throw exception("fovea::linear_space(invalid item size)");
            return sz;
        }
        
        
        linear_space:: linear_space(const size_t     num_items,
                                    const size_t     item_size,
                                    const type_spec &items_sp,
                                    const type_spec &super_sp):
        entry(0),
        items( __check_linear_items(num_items)  ),
        itmsz( __check_linear_sizeof(item_size) ),
        bytes(items*itmsz),
        items_type(items_sp),
        array_type(super_sp),
        allocated(bytes)
        {
            try
            {
                entry = memory::kind<memory::global>::acquire(allocated);
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        
        
    }
}
