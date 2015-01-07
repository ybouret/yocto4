#ifndef YOCTO_GFX_RAWPIX_HIST_INCLUDED
#define YOCTO_GFX_RAWPIX_HIST_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"

namespace yocto
{
    namespace gfx
    {
        
        
        class histogram
        {
        public:
            static const size_t bins = 256;
            double count[bins];
            
            static uint8_t level( const uint8_t &u ) throw() { return u; }
            static uint8_t level( const float   &f ) throw() { return conv::to_byte(f); }
            static uint8_t level( const double  &f ) throw() { return conv::to_byte(float(f)); }
            static uint8_t level( const rgb_t   &c ) throw() { return conv::greyscale(c.r,c.g,c.b); }
            static uint8_t level( const rgba_t  &c ) throw() { return conv::greyscale(c.r,c.g,c.b); }
            
            
            
            inline   histogram() throw() : count() {  clear(); }
            virtual ~histogram() throw() {}
            
            inline void clear() throw() {for(size_t i=0;i<bins;++i) count[i] = 0; }
            
            double &       operator[](size_t i) throw()       { assert(i<bins); return count[i]; }
            const double & operator[](size_t i) const throw() { assert(i<bins); return count[i]; }
            
            
            size_t threshold() const throw();
            
            template <typename T>
            inline void compute_from(const pixmap<T> &pxm) throw()
            {
                clear();
                const unit_t w = pxm.w;
                const unit_t h = pxm.h;
                
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Rj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        ++count[ level(Rj[i]) ];
                    }
                }
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };
        
        struct threshold
        {
            enum mode_type
            {
                keep_white,
                keep_black
            };
            
            template <typename T>
            inline static
            void apply( pixmap<T> &target, const size_t t, const pixmap<T> &source, mode_type m = keep_white ) throw()
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
                        const size_t   lvl = histogram::level(src);
                        T             &tgt = Tj[i];
                        switch(m)
                        {
                            case keep_white:
                                if(lvl<=t)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = src;
                                }
                                break;
                                
                            case keep_black:
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
