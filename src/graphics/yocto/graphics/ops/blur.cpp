#include "yocto/graphics/ops/blur.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    using namespace math;

    namespace graphics
    {

        blur:: ~blur() throw()
        {
        }

        blur:: blur(float sig) :
        width(0),
        top(0),
        weight(0),
        __data()
        {
            // TODO: check sig
            const float sig2    = sig*sig;
            const float twosig2 = sig2+sig2;
            const float exp_min = numeric<float>::epsilon * sqrtf(numeric<float>::two_pi*sig2);
            const float len_max = (exp_min >= 1.0f) ? 0.0f : sqrtf( - twosig2 * logf(exp_min) );
            std::cerr << "len_max=" << len_max << std::endl;
            (unit_t &)width = max_of<unit_t>(1,ceilf(len_max));
            (unit_t &)top   = width-1;
            std::cerr << "width=" << width << std::endl;
            __data.make(width+max_of<unit_t>(1,top*top));
            weight = & __data[1];
            for(unit_t i=0;i<width;)
            {
                const float X   = float(i);
                const float arg = (X*X)/twosig2;
               __data[++i] = exp(-arg);
            }

            std::cerr << "weights=";
            for(unit_t i=0;i<width;++i) std::cerr << " " << weight[i];
            std::cerr << std::endl;
            

        }
        

    }
}
