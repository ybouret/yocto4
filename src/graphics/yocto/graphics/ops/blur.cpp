
#include "yocto/graphics/ops/blur.hpp"
#include <cfloat>

namespace yocto {
    
    namespace graphics
    {
        blur::patch::patch(const graphics::patch &p) throw() :
        graphics::patch(p),
        handle(0),
        target(0),
        source(0)
        {
        }
        
        blur::patch:: ~patch() throw()
        {
        }
        
        blur::patch::patch(const patch &p) throw() :
        graphics::patch(p),
        handle(0),
        target(0),
        source(0)
        {
            
        }

        void blur::patch:: compute( lockable & ) throw()
        {
            assert(handle);
            blur &blr = *static_cast<blur *>(handle);
            const unit_t delta   = blr.delta;
            const float  scaling = blr.scaling;
            const unit_t jtop    = min_of(upper.y,delta);
            const unit_t itop    = min_of(upper.x,delta);
            for(unit_t j=lower.y;j<=jtop;++j)
            {
                const unit_t j2 = j*j;
                for(unit_t i=lower.x;i<=itop;++i)
                {
                    const unit_t i2  = i*i;
                    const float  arg = float(i2+j2)/scaling;
                    blr[j][i] = std::exp(-arg);
                }
            }
        }

    }
    
}

namespace yocto {

    namespace graphics
    {

        blur:: ~blur() throw()
        {
        }


        blur:: blur(const graphics::patch &surface, threading::engine *server) :
        pixmap<float>(surface.width.x,surface.width.y),
        patches(),
        scaling(0),
        delta(0)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(patches, cpus, *this, true);
        }


        void blur:: compute(const float sigma, threading::engine *server)
        {
            const double sig2 = sigma*sigma;
            scaling = sig2+sig2;
            delta   = unit_t(std::ceil( std::sqrt( -std::log(FLT_EPSILON) * scaling ) ));
            std::cerr << "delta(" << sigma << ")=" << delta << std::endl;

            faked_lock sequential;
            for(size_t i=patches.size();i>0;--i)
            {
                patch &sub = patches[i];
                sub.handle = this;
                sub.target = 0;
                sub.source = 0;
                if(server)
                {
                    server->enqueue(&sub, &blur::patch::compute);
                }
                else
                {
                    sub.compute(sequential);
                }
            }
            if(server) server->flush();

        }


    }

}
