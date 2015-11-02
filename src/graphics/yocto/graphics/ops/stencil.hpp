#ifndef YOCTO_STENCIL_OPS_INCLUDED
#define YOCTO_STENCIL_OPS_INCLUDED 1

#include "yocto/graphics/pixmaps.hpp"

namespace yocto
{
    namespace graphics
    {
        //! a 3x3 stencil
        class stencil
        {
        public:
            static const int at_left   = 0x01;
            static const int at_right  = 0x02;
            static const int at_bottom = 0x04;
            static const int at_top    = 0x08;

            enum filling
            {
                fill_with_zero,
                fill_with_same,
                fill_with_symm,
                fill_with_asym,
                fill_no_fluxes
            };

            typedef float real_type;
            void ldz() throw();
            
            stencil() throw();
            stencil(const stencil &other) throw();
            stencil & operator=(const stencil &) throw();
            ~stencil() throw();

            filling   mode;
            real_type weight[3][3];

            inline real_type & operator()(unit_t dx, unit_t dy) throw()
            {
                assert(dx>=-1);assert(dx<=1);assert(dy>=-1);assert(dy<=1);
                return weight[++dx][++dy];
            }

            inline const real_type & operator()(unit_t dx, unit_t dy) const throw()
            {
                assert(dx>=-1);assert(dx<=1);assert(dy>=-1);assert(dy<=1);
                return weight[++dx][++dy];
            }


            template <typename T>
            void load(const pixmap<T> &pxm,
                      const unit_t     i,
                      const unit_t     j) throw()
            {
                assert(pxm.w>=3);
                assert(pxm.h>=3);
                stencil        &self   = *this;
                const real_type center = real_type(pxm[j][i]);
                const unit_t w0 = pxm.upper.x;
                const unit_t h0 = pxm.upper.y;
                self(0,0)       = center;
                int flag  = 0;
                if(i<=0)
                {
                    flag |= at_left;
                }
                else
                {
                    if(i>=w0)
                    {
                        flag |= at_right;
                    }
                }
                if(j<=0)
                {
                    flag |= at_bottom;
                }
                else
                {
                    if(j>=h0)
                    {
                        flag |= at_top;
                    }
                }

                switch(flag)
                {
                    case 0: break;

                    case at_left:
                        break;

                    case at_right:
                        break;

                    case at_top:
                        break;

                    case at_bottom:
                        break;

                    case at_left | at_top:
                        break;

                    case at_left | at_bottom:
                        break;

                    case at_right | at_top:
                        break;

                    case at_right | at_bottom:
                        break;

                    default:
                        assert(die("never get here"));
                        break;
                }

            }
            

        };
    }
}

#endif
