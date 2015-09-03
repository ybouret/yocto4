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


}