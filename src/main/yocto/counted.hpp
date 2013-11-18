#ifndef YOCTO_COUNTED_INCLUDED
#define YOCTO_COUNTED_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    class counted
    {
    public:
        inline virtual ~counted() throw() { assert(nref_==0); }
        
        inline void   withhold() throw() { ++nref_; }
        inline bool   liberate() throw() { assert(nref_>0); return --nref_ <= 0; }
        inline size_t refcount() const throw() { return nref_; }
        
    protected:
        inline explicit counted() throw() : nref_(0) {}
        
    private:
        size_t nref_;
        YOCTO_DISABLE_COPY_AND_ASSIGN(counted);
    };
}

#endif
