#ifndef YOCTO_STRING_B64ID_INCLUDED
#define YOCTO_STRING_B64ID_INCLUDED 1

#include "yocto/string/base64.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/binary.hpp"

#include <cstring>

namespace yocto
{
    template <typename T>
    class b64id
    {
    public:
        static const size_t input_bits = sizeof(T)<<3;
        static const size_t min_bytes  = input_bits/6;
        static const size_t bytes      = YOCTO_ROUND1(min_bytes);
        const char label[bytes+1];

        inline b64id(const T *pod) throw() :
        label()
        {
            memset((void*)label,0,sizeof(label));
            make_label(pod);
        }

        inline ~b64id() throw()
        {
        }

        inline b64id(const b64id &other) throw() :
        label()
        {
            memcpy((void*)label,other.label,sizeof(label));
        }



    private:
        YOCTO_DISABLE_ASSIGN(b64id);
        inline void make_label(const void *data) throw()
        {
            char  *s=(char*)label;
            
        }
        
    };
    
    
}

#endif
