#ifndef YOCTO_COUNTED_OBJECT_INCLUDED
#define YOCTO_COUNTED_OBJECT_INCLUDED 1

#include "yocto/counted.hpp"
#include "yocto/object.hpp"


namespace yocto
{
    class counted_object : public object, public counted
    {
    public:
        inline explicit counted_object() throw() :object(),counted() {}
        inline virtual ~counted_object() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(counted_object);
    };
}
#endif
