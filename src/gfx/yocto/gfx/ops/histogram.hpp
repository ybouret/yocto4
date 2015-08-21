#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/code/bzset.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {



        //!
        class histogram
        {
        public:
            static const size_t bins = 256;
            size_t  classes;
            double  count[bins];
            uint8_t bin[bins]; //! 0..classes-1
            double  cdf[bins]; //! 0..classes-1
            uint8_t lut[bins]; //! Look Up Table



            explicit histogram() throw();
            virtual ~histogram() throw();

            void   reset() throw();
            void   build_cdf() throw();
            double icdf(const double y,const double gam) const throw();

            void build_lut(const double gam) throw();


            template <typename T>
            void append( const pixmap<T> &px, uint8_t (*item2byte)(const T&) ) throw()
            {
                assert(item2byte);
                const unit_t w = px.w;
                const unit_t h = px.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Rj = px[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        const uint8_t w = item2byte( Rj[i] );
                        ++count[w];
                    }
                }
            }

            template <typename T>
            void build_for(const pixmap<T> &px, uint8_t (*item2byte)(const T&)) throw()
            {
                reset();
                append(px,item2byte);
                build_cdf();
            }


            size_t threshold() const throw(); //! Otsu, to apply threshold

            template <typename U,typename T>
            void apply_lut(pixmap<U> &target,
                           U (*byte2data)(const uint8_t &),
                           const pixmap<T> &source,
                           uint8_t (*item2byte)(const T&) ) throw()
            {
                assert(target.w==source.w);
                assert(target.h==source.h);
                assert(byte2data);
                assert(item2byte);
                const unit_t w=target.w;
                const unit_t h=target.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Sj = source[j];
                    typename pixmap<U>::row       &Tj = target[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        Tj[i] = byte2data( lut[ item2byte(Sj[i]) ] );
                    }
                }
            }

            template <typename U,typename T>
            inline void equalize(pixmap<U> &target,
                                 U (*byte2data)(const uint8_t &),
                                 const pixmap<T> &source,
                                 uint8_t (*item2byte)(const T&),
                                 const double gam) throw()
            {
                this->build_for<T>(source,item2byte);
                this->build_lut(gam);
                this->apply_lut<U,T>(target,byte2data,source,item2byte);
            }

            inline void equalize8(pixmap<uint8_t> &px,const double gam)
            {
                this->equalize(px,to_byte<uint8_t>,px,to_byte<uint8_t>,gam);
            }
            
            inline void equalizef(pixmap<float> &px, const double gam)
            {
                this->equalize(px,to_float,px,to_byte<float>, gam);
            }

            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };

        struct threshold
        {
            enum mode_type
            {
                keep_foreground,
                keep_background
            };

            template <typename T>
            inline static
            void apply(pixmap<T>      &target,
                       const size_t     t,
                       const pixmap<T> &source,
                       uint8_t        (*addr2byte)(const T&),
                       mode_type        m ) throw()
            {
                assert(source.w==target.w);
                assert(source.h==target.h);
                const unit_t w = source.w;
                const unit_t h = source.h;

                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Sj = source[j];
                    typename       pixmap<T>::row &Tj = target[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        const T       &src = Sj[i];
                        const size_t   lvl = addr2byte(src);
                        T             &tgt = Tj[i];
                        switch(m)
                        {
                            case keep_foreground:
                                if(lvl<=t)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = src;
                                }
                                break;

                            case keep_background:
                                if(t<=lvl)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = invert_color(src);
                                }
                                break;
                        }
                    }
                }
                
            }
        };
        
    }
}

#endif
