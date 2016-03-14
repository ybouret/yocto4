#ifndef YOCTO_CONTAINER_FIELD_INCLUDED
#define YOCTO_CONTAINER_FIELD_INCLUDED 1

#include "yocto/parallel/patch.hpp"

namespace yocto
{

    template <typename T, typename COORD>
    class field_of : public parallel::patch_of<COORD>
    {
    public:
        typedef parallel::patch_of<COORD> patch_type;
        static const size_t DIM = patch_type::DIM;

        inline virtual ~field_of() throw()
        {
        }

    protected:
        T *entry;
        explicit field_of( const patch_type &p ) : patch_type(p), entry(0) {}



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(field_of);
    };

    
    template <typename T>
    class field1D : public field_of<T,parallel::coord1D>
    {
    public:



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(field1D);
    };


}


#endif

