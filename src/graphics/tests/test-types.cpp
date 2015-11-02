#include "yocto/graphix/types.hpp"
#include "yocto/utest/run.hpp"
//#include "yocto/sequence/vector.hpp"
//#include "yocto/sort/unique.hpp"

#include "yocto/ordered/sorted-vector.hpp"

using namespace yocto;
using namespace graphix;

YOCTO_UNIT_TEST_IMPL(types)
{

    if(false)
    {
        const size_t nmax = 16777216;
        sorted_vector<float> gsv(nmax,as_capacity);
        size_t count = 0;
        size_t uniqv = 0;
        for(int r=0;r<256;++r)
        {
            for(int g=0;g<256;++g)
            {
                for(int b=0;b<256;++b)
                {

                    const float gs = gist::greyscalef(r, g, b);
                    if(gs<0.0f)
                        throw exception("negative value for %d,%d,%d",r,g,b);
                    if(gs>1.0f)
                        throw exception("too big value for %d,%d,%d",r,g,b);
                    ++count;
                    if(gsv.insert(gs)) ++uniqv;
                }
            }
            const double percent = (100.0*count)/nmax;
            fprintf(stderr,"%7.3lf%%   => %9u/%9u   \r", percent, unsigned(uniqv), unsigned(count) );
            fflush(stderr);
        }
        fprintf(stderr,"\n");
    }
}
YOCTO_UNIT_TEST_DONE()