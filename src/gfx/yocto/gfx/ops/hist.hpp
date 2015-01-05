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
            
            explicit histogram() throw() : count() {  clear(); }
            virtual ~histogram() throw() {}
            
            void clear() throw() {for(size_t i=0;i<bins;++i) count[i] = 0; }
            
            double &       operator[](size_t i) throw()       { assert(i<bins); return count[i]; }
            const double & operator[](size_t i) const throw() { assert(i<bins); return count[i]; }

            void store( const uint8_t &u ) throw() { ++count[u]; }
            void store( const float   &f ) throw() { ++count[ conv::to_byte(f) ]; }
            void store( const double  &f ) throw() { ++count[ conv::to_byte(float(f)) ]; }
            void store( const rgb_t   &c ) throw() { ++count[ conv::greyscale(c.r,c.g,c.b) ]; }
            void store( const rgba_t  &c ) throw() { ++count[ conv::greyscale(c.r,c.g,c.b) ]; }

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };
        
        
        template <typename T>
        inline void compute_histogram(histogram &H, const pixmap<T> &pxm)
        {
            H.clear();
            const unit_t w = pxm.w;
            const unit_t h = pxm.h;
            
            for(unit_t j=0;j<h;++j)
            {
                const typename pixmap<T>::row &Rj = pxm[j];
                for(unit_t i=0;i<w;++i)
                {
                    H.store( Rj[i] );
                }
            }
            
        }
        
    }
}


#endif
