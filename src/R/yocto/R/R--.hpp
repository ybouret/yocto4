#ifndef YOCTO_RMM_INCLUDED
#define YOCTO_RMM_INCLUDED 1

#include "yocto/os.hpp"
#include "yocto/memory/global.hpp"

#include <new>

namespace yocto
{
#define YOCTO_R_ARRAY_ACCESS(ADDR) \
inline virtual T operator[](const size_t indx) throw() { assert(indx<this->size()); return ADDR[indx]; }
    
    template <typename T>
    class RArray
    {
    public:
        
        virtual ~RArray() throw() {}
        virtual  size_t size() const throw() = 0;
        
    protected:
        inline RArray() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(RArray);
    };

    
    template <typename T>
    class CVector : public RArray<T>
    {
    public:
        explicit CVector(size_t n) :
        items(n),
        inMem(n),
        data( memory::kind<memory::global>::acquire_as<T>(inMem) )
        {
        }
        
        virtual ~CVector() throw()
        {
            memory::kind<memory::global>::release_as<T>(data,inMem);
        }
        
        inline virtual size_t size() const throw() { return items; }
        
        YOCTO_R_ARRAY_ACCESS(data)
        
    private:
        const size_t items;
        size_t       inMem;
        T           *data;
    };
}

#endif
