#ifndef YOCTO_GRAPHICS_OPS_FFT_INCLUDED
#define YOCTO_GRAPHICS_OPS_FFT_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"

namespace yocto
{
    namespace graphics
    {
        class fft
        {
        public:
            //! must be a full pixmap, not shared
            static void __forward( pixmapz &img ) throw();

            //! must be a full pixmap, not shared
            static void __reverse( pixmapz &img ) throw();

            explicit fft( const unit_t user_w, const unit_t user_h);
            virtual ~fft() throw();


            pixmapz         data;  //!< next_power_of_two(2*user_w,2*user_h)
            pixmapf         core;  //!< shared on lower left quadrant.
            



            //! put src into core, zooming
            void load( const pixmapf &src ) throw();

            //! dispatch core into data
            void dispatch() throw();

            //! forward FFT of data
            void forward() throw();

            //! reverse FFT of data
            void reverse() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fft);
        };
    }
}

#endif
