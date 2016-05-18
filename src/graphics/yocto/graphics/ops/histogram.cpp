#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cstring>

namespace yocto
{
    namespace graphics
    {

        Histogram:: Histogram() throw() :
        count()
        {
            reset();
        }

        void Histogram:: reset() throw()
        {
            memset(count,0,sizeof(count));
        }

        void Histogram:: save(const string &hname) const
        {
            ios::wcstream fp(hname);
            for(size_t i=0;i<bins;++i)
            {
                fp("%u %g\n", unsigned(i), double(count[i]));
            }
        }

        void Histogram:: save(const char *hname) const
        {
            const string id(hname);
            save(id);
        }

        void Histogram:: collect( const Histogram &H ) throw()
        {
            for(size_t i=0;i<bins;++i)
            {
                count[i] += H.count[i];
            }
        }


    }

}

