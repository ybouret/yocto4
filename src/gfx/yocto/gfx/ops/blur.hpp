#ifndef YOCTO_GFX_OPS_BLUR_INCLUDED
#define YOCTO_GFX_OPS_BLUR_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/utils.hpp"
#include <cstdlib>

namespace yocto
{
    namespace gfx
    {

        class blur_info
        {
        public:
            virtual ~blur_info() throw();

            const unit_t __width;

        protected:
            explicit blur_info(const float sig);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blur_info);
        };

        class blur : public blur_info, public pixmap<float>
        {
        public:
            explicit blur(const float sig);
            virtual ~blur() throw();

            const unit_t  top;      //!< width-1


            template <
            typename T,
            typename U,
            size_t   NCH>
            inline void __apply(pixmap<T>         &tgt,
                                const pixmap<T>   &src,
                                xpatches          &xps,
                                threading::engine *server
                                )
            {
                source = &src;
                target = &tgt;
                for(size_t p=xps.size();p>0;--p)
                {
                    xpatch &xp = xps[p];
                    xp.enqueue(this, & blur::eval<T,U,NCH>, server);
                }
                if(server) server->flush();
            }


            inline void apply(pixmap<float>       &tgt,
                              const pixmap<float> &src,
                              xpatches            &xps,
                              threading::engine   *server
                              )
            {
                __apply<float, float, 1>(tgt, src, xps, server);
            }

            inline void apply(pixmap<RGB>       &tgt,
                              const pixmap<RGB> &src,
                              xpatches            &xps,
                              threading::engine   *server
                              )
            {
                __apply<RGB, uint8_t, 3>(tgt, src, xps, server);
            }

        private:

            YOCTO_DISABLE_COPY_AND_ASSIGN(blur);

            void              *target;
            const void        *source;


            template <typename T, typename U, size_t NCH>
            inline void eval( xpatch &xp, lockable & ) throw()
            {
                assert(target);
                assert(source);
                const pixmap<T> &src  = *static_cast<const pixmap<T>*>(source);
                pixmap<T>       &tgt  = *static_cast< pixmap<T>     *>(target);

                const int        len  = top;
                const unit_t     xup  = src.upper.x;
                const unit_t     yup  = src.upper.y;


                float sum[NCH];
                //______________________________________________________________
                //
                // ENTER: loop over rows
                //______________________________________________________________
                for(unit_t j=xp.lower.y;j<=xp.upper.y;++j)
                {
                    typename pixmap<T>::row &tgt_j = tgt[j];

                    const int jinf = -int(min_of<unit_t>(len,j));
                    const int jsup =  int(min_of<unit_t>(len,yup-j));

                    //__________________________________________________________
                    //
                    // ENTER: loop over columns
                    //__________________________________________________________
                    for(unit_t i=xp.lower.x;i<=xp.upper.x;++i)
                    {
                        const int iinf = -int(min_of<unit_t>(len,i));
                        const int isup =  int(min_of<unit_t>(len,xup-i));

                        //______________________________________________________
                        //
                        // initialize mask
                        //______________________________________________________
                        for(size_t ch=0;ch<NCH;++ch)
                        {
                            sum[ch] = 0;
                        }
                        float acc = 0;

                        //______________________________________________________
                        //
                        // loop over mask
                        //______________________________________________________
                        for(int jj=jinf;jj<=jsup;++jj)
                        {
                            const int                      jjj    = abs(jj);
                            const typename pixmap<T>::row &src_jj = src[j+jj];
                            const pixmap<float>::row      &coef_j = (*this)[jjj];

                            for(int ii=iinf;ii<=isup;++ii)
                            {
                                const U     *p    = (const U *) &src_jj[i+ii];
                                const int    iii  = abs(ii);
                                const float  coef = coef_j[iii];
                                for(size_t ch=0;ch<NCH;++ch)
                                {
                                    sum[ch] += float(p[ch]) * coef;
                                }
                                acc += coef;
                            }
                        }

                        //______________________________________________________
                        //
                        // compute new value(s)
                        //______________________________________________________
                        acc  = 1.0f/acc;
                        U *q = (U *)&tgt_j[i];
                        for(size_t ch=0;ch<NCH;++ch)
                        {
                            q[ch] = U(sum[ch]*acc);
                        }

                    }
                    //__________________________________________________________
                    //
                    // LEAVE: loop over columns
                    //__________________________________________________________
                }
                //______________________________________________________________
                //
                // LEAVE: loop over rows
                //______________________________________________________________
                
            }
            
            
        };

        template <typename T>
        inline void apply_blur(const float          sig,
                               pixmap<T>           &dst,
                               const pixmap<T>     &src,
                               xpatches            &xps,
                               threading::engine   *server)
        {
            blur B(sig);
            B.apply(dst,src,xps,server);
        }


    }
}

#endif
