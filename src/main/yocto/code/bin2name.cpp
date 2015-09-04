#include "yocto/code/bin2name.hpp"

namespace yocto
{
    const char * bin2name::head[16] =
    {
        "B",
        //"c",
        "D",
        "F",
        "G",
        "H",
        "J",
        "K",
        "L",
        "M",
        "N",
        "P",
        //"q",
        "R",
        "S",
        "T",
        "V",
        //"w",
        "X",
        //"z"
    };

    const char * bin2name::tail[16] =
    {
        "aa",
        "ae",
        "ai",
        "ao",
        "au",
        "ay",

        "ea",
        "ee",
        "ei",
        "eo",
        "eu",
        "ey",

        "ia",
        "ie",
        "io",
        "iu"
    };

    const char *bin2name:: get_head(const unsigned B) throw()
    {
        assert(16==sizeof(head)/sizeof(head[0]));
        return head[ (B&0xF0) >> 4 ];
    }

    const char *bin2name:: get_tail(const unsigned B) throw()
    {
        assert(16==sizeof(tail)/sizeof(tail[0]));
        return tail[ (B&0x0F) ];
    }

    void  bin2name:: fill(char *value, key_t k) throw()
    {
        assert(value);
        memset(value,0,mem_size);
        for(size_t i=0;i<sizeof(key_t);++i)
        {
            const size_t   j = i*3;
            const unsigned B = (k&0xff);
            const char    *h = get_head(B);
            const char    *t = get_tail(B);
            value[j+0] = h[0];
            value[j+1] = t[0];
            value[j+2] = t[1];
            k >>= 8;
        }
    }

    void  bin2name::fill(char *value, const void *data, const size_t size) throw()
    {
        fill(value,key_of(data,size));
    }

}

#include "yocto/hashing/sha1.hpp"

namespace yocto
{
    bin2name::key_t bin2name::key_of(const void *data, const size_t size) throw()
    {
        hashing::sha1 H;
        return H.key<key_t>(data,size);
    }

}

