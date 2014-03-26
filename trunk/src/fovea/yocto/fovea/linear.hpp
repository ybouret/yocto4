#ifndef YOCTO_FOVEA_LINEAR_INCLUDED
#define YOCTO_FOVEA_LINEAR_INCLUDED 1

#include "yocto/fovea/types.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace fovea
    {
        //! memory for fields
        class linear_space : public object, public counted
        {
        public:
            void           *entry;
            const size_t    items;
            const size_t    itmsz;      //!< item size in bytes
            const size_t    bytes;      //!< items * itmz
            const type_spec items_type; //!< items class type
            const type_spec array_type; //!< super class type

            virtual ~linear_space() throw();
            
            virtual void *array_handle() const throw() = 0; //!< address of superclass
            
            template <typename ARRAY>
            ARRAY & as() throw()
            {
                assert(array_type.match(typeid(ARRAY)));
                return *static_cast<ARRAY *>(array_handle());
            }
            
            template <typename ARRAY>
            const ARRAY & as() const throw()
            {
                assert(array_type.match(typeid(ARRAY)));
                return *static_cast<ARRAY *>(array_handle());
            }
            
        protected:
            explicit linear_space(const size_t     num_items,
                                  const size_t     item_size,
                                  const type_spec &items_sp,
                                  const type_spec &super_sp);
            
        private:
            size_t allocated;
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_space);
            void   clear() throw();
        };
    }
}


#endif
