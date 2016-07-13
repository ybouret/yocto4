#include "yocto/gfx/types.hpp"
#include "yocto/utest/run.hpp"
#include <cstdio>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(types)
{

    for(int i=0;i<256;++i)
    {
        if(i<=0)
        {
            printf("%3d.0f       ",0);
        }
        else
        {
            if(i>=255)
            {
                printf("%3d.0f       ",1);
            }
            else
            {
                printf("%3d.0f/255.0f",i);
            }
        }
        if(i<255) printf(",");
        if(0==(1+i)%8) printf("\n"); else printf(" ");
    }
    printf("\n");

}
YOCTO_UNIT_TEST_DONE()
