#ifndef YOCTO_GFX_PARALLEL_INCLUDED
#define YOCTO_GFX_PARALLEL_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/threading/simd.hpp"

namespace yocto
{
    namespace gfx
    {

        class patch
        {
        public:
            bitmap          &bmp;
            const rectangle  rect;

            explicit patch(bitmap &user_bmp, const rectangle &user_rect) throw();
            virtual ~patch() throw();

            //! create patches according to #threads and bmp
            /**
             \param simd a SIMD structure
             \param bmp  a bitmap to process
             \param full if false, exclude borders
             */
            static void setup(threading::SIMD &simd,
                              bitmap          &bmp,
                              const bool       full);
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(patch);
        };


    }
}

#endif
