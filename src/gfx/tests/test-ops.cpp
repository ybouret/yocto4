#include "yocto/utest/run.hpp"
#include "yocto/gfx/image/jpeg.hpp"
#include "yocto/gfx/image/png.hpp"
#include "yocto/gfx/image/tiff.hpp"
#include "yocto/gfx/ops/channels.hpp"
#include "yocto/gfx/ops/gradient.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/sys/timings.hpp"

using namespace yocto;
using namespace gfx;

namespace
{
#define TMX 2
    static inline double split_speed(pixmaps<uint8_t>   &ch,
                                     const pixmap3      &px,
                                     const size_t        cpus,
                                     threading::engine  *psrv)
    {
        timings tmx;
        YOCTO_TIMINGS(tmx, TMX,
                      channels<>::iopatches chn_ops;
                      chn_ops.setup_for(cpus,px);
                      chn_ops.split(ch,px,psrv)
                      );
        return tmx.speed;
    }


    static inline double grad_speed(pixmaps<uint8_t>       &grad,
                                    const pixmaps<uint8_t> &data,
                                    const size_t            cpus,
                                    threading::engine      *psrv)
    {
        assert(grad.size==data.size);
        typedef gradient::real_type real_type;
        timings tmx;
        const size_t nch = grad.size;
        YOCTO_TIMINGS(tmx, TMX,
                      gradient::ipatches igr;
                      gradient::opatches ogr;
                      gradient::setup(igr, ogr, cpus, data[0]);
                      pixmap<real_type> G(data.w,data.h);
                      for(size_t i=0;i<nch;++i)
                      {
                          const real_type Gmax = gradient::compute1(igr, G, data[i], psrv);
                          gradient::compute2<uint8_t>(ogr, grad[i], G, Gmax, psrv);
                      }
                      );
        return tmx.speed;
    }

    static inline double merge_speed(pixmap3                  &px,
                                     const pixmaps<uint8_t>   &ch,
                                     const size_t              cpus,
                                     threading::engine        *psrv)
    {
        timings tmx;
        YOCTO_TIMINGS(tmx, TMX,
                      channels<>::iopatches chn_ops;
                      chn_ops.setup_for(cpus,px);
                      chn_ops.merge(px,ch,psrv)
                      );
        return tmx.speed;
    }


}

YOCTO_UNIT_TEST_IMPL(ops)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    const image::format &PNG = IMG["PNG"];

    //threading::SIMD simd;

    threading::engine psrv;
    if(argc>1)
    {
        const string         filename = argv[1];

        // load image
        bitmap::pointer bmp(IMG.load(filename,3, put_rgba::to_rgb,NULL,NULL));

        // make a pixmap, RGB
        pixmap3         pxm(bmp,NULL);
        PNG.save("image3.png",pxm, get_rgba::from_rgb,NULL, NULL);

        // start ops
        const unit_t w = pxm.w;
        const unit_t h = pxm.h;
        std::cerr << "-- w=" << w << ", h=" << h << std::endl;

        // splitting
        std::cerr << "\tSPLITTING" << std::endl;
        pixmaps<uint8_t> ch(3,w,h);
        const double split_seq1 = split_speed(ch, pxm, 1, NULL);
        std::cerr << "split_seq1=" << split_seq1 << std::endl;
        const double split_seqn = split_speed(ch, pxm, psrv.size, NULL);
        std::cerr << "split_seqn=" << split_seqn << std::endl;
        const double split_para = split_speed(ch, pxm, psrv.size, &psrv);
        std::cerr << "split_para=" << split_para << std::endl;
        std::cerr << "\tsplit_factor : " << split_para/min_of(split_seq1,split_seqn) << std::endl;


        PNG.save("col_image_r.png",ch[0], get_rgba::from_byte_r,NULL, NULL);
        PNG.save("col_image_g.png",ch[1], get_rgba::from_byte_g,NULL, NULL);
        PNG.save("col_image_b.png",ch[2], get_rgba::from_byte_b,NULL, NULL);

        // gradient
        std::cerr << "\tGRADIENT" << std::endl;
        pixmaps<uint8_t> gr(3,w,h);
        const double grad_seq1 = grad_speed(gr,ch,1,NULL);
        std::cerr << "grad_seq1=" << grad_seq1 << std::endl;
        const double grad_seqn = grad_speed(gr,ch,psrv.size,NULL);
        std::cerr << "grad_seqn=" << grad_seqn << std::endl;
        const double grad_para = grad_speed(gr,ch,psrv.size,&psrv);
        std::cerr << "grad_para=" << grad_para << std::endl;
        std::cerr << "\tgrad_factor : " << grad_para/min_of(grad_seq1,grad_seqn) << std::endl;


        PNG.save("grad_image_r.png",gr[0], get_rgba::from_byte_r,NULL, NULL);
        PNG.save("grad_image_g.png",gr[1], get_rgba::from_byte_g,NULL, NULL);
        PNG.save("grad_image_b.png",gr[2], get_rgba::from_byte_b,NULL, NULL);

        // merging
        std::cerr << "\tMERGING" << std::endl;
        pixmap3 pxm2(w,h);
        const double merge_seq1 = merge_speed(pxm2, gr, 1, NULL);
        std::cerr << "merge_seq1=" << merge_seq1 << std::endl;
        const double merge_seqn = merge_speed(pxm2, gr, psrv.size, NULL);
        std::cerr << "merge_seqn=" << merge_seqn << std::endl;
        const double merge_para = merge_speed(pxm2, gr, psrv.size, &psrv);
        std::cerr << "merge_para=" << merge_para << std::endl;
        std::cerr << "\tmerge_factor : " << merge_para/min_of(merge_seq1,merge_seqn) << std::endl;

        PNG.save("grad_image2.png",pxm2, get_rgba::from_rgb,NULL, NULL);


    }

    
}
YOCTO_UNIT_TEST_DONE()
