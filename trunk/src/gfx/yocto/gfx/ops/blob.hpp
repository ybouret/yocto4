#ifndef YOCTO_GFX_BLOB_INCLUDED
#define YOCTO_GFX_BLOB_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"

namespace yocto
{
    namespace gfx
    {
        typedef pixmap<size_t> blob_type;
        
        class blob : public blob_type
        {
        public:
            
            
            typedef blob_type::type word_t;
            word_t                  count;
            template <typename T>
            blob( const pixmap<T> &src, bool detect8 = true) :
            blob_type(src.w,src.h),
            count(0)
            {
                const unit_t H  = h;
                
                // first pass
                for(unit_t j=0,jp=1,jm=-1;j<H;++j,++jp,++jm)
                {
                    if( !(j&1) )
                    {
                        scan_increase(src, j, jm, jp, detect8);
                    }
                    else
                    {
                        scan_decrease(src, j, jm, jp, detect8);
                    }
                }
            }
            
            
            virtual ~blob() throw()
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            
            template <typename T> inline
            void scan_increase(const pixmap<T> &src,
                               const unit_t j,
                               const unit_t jm,
                               const unit_t jp,
                               const bool   detect8)
            {
                const typename pixmap<T>::row &Sj         = src[j];
                blob_type::row                &Bj         = (*this)[j];
                const unit_t W          = w;
                const bool   has_bottom = jp<h;
                for(unit_t i=0,ip=1,im=-1;i<W;++i,++ip,++im)
                {
                    if(!is_zero_pixel(Sj[i]))
                    {
                        
                        //__________________________________________________
                        //
                        // check status
                        //__________________________________________________
                        word_t &b = Bj[i];
                        if(b<=0)
                        {
                            //not visited
                            b = ++count;
                        }
                        const word_t id = b;
                        
                        //__________________________________________________
                        //
                        // propagate right
                        //__________________________________________________
                        const bool has_right = ip<W;
                        if(has_right)
                        {
                            if(!is_zero_pixel(Sj[ip]))
                            {
                                Bj[ip] = id;
                            }
                        }
                        
                        const bool has_left = i>0;
                        //__________________________________________________
                        //
                        // propagate bottom
                        //__________________________________________________
                        if(has_bottom)
                        {
                            check_bottom(id, jp, i, im, ip, src, detect8, has_left, has_right);
                        }
                        
                    }
                }
            }
            
            template <typename T> inline
            void scan_decrease(const pixmap<T> &src,
                               const unit_t     j,
                               const unit_t     jm,
                               const unit_t     jp,
                               const bool       detect8)
            {
                const typename pixmap<T>::row &Sj         = src[j];
                blob_type::row                &Bj         = (*this)[j];
                const unit_t W          = w;
                const bool   has_bottom = jp<h;
                
                for(unit_t i=W-1,ip=W,im=W-2;i>=0;--i,--ip,--im)
                {
                    if(!is_zero_pixel(Sj[i]))
                    {
                        
                        //__________________________________________________
                        //
                        // check status
                        //__________________________________________________
                        word_t &b = Bj[i];
                        if(b<=0)
                        {
                            //not visited
                            b = ++count;
                        }
                        const word_t id = b;
                        
                        //__________________________________________________
                        //
                        // propagate left
                        //__________________________________________________
                        
                        const bool has_left = i>0;
                        if(has_left)
                        {
                            if(!is_zero_pixel(Sj[im]))
                            {
                                Bj[im] = id;
                            }
                        }
                        
                        const bool has_right = ip<W;
                        
                        //__________________________________________________
                        //
                        // propagate bottom
                        //__________________________________________________
                        if(has_bottom)
                        {
                            check_bottom(id, jp, i, im, ip, src, detect8, has_left, has_right);
                        }
                        
                    }
                }
            }
            
            
            template <typename T> inline
            void check_bottom(const size_t     id,
                              const unit_t     jp,
                              const unit_t     i,
                              const unit_t     im,
                              const unit_t     ip,
                              const pixmap<T> &src,
                              const bool       detect8,
                              const bool       has_left,
                              const bool       has_right)
            {
                const typename pixmap<T>::row &Sjp = src[jp];
                blob_type::row                &Bjp = (*this)[jp];
                
                if(!is_zero_pixel(Sjp[i]))
                {
                    Bjp[i] = id;
                }
                
                if(detect8)
                {
                    //__________________________________________
                    //
                    //propagate bottom/right
                    //__________________________________________
                    if(has_right)
                    {
                        if(!is_zero_pixel(Sjp[ip]))
                        {
                            Bjp[ip] = id;
                        }
                    }
                    
                    //__________________________________________
                    //
                    //propagate bottom/left
                    //__________________________________________
                    if(has_left)
                    {
                        if(!is_zero_pixel(Sjp[im]))
                        {
                            Bjp[im] = id;
                        }
                    }
                    
                }
                
            }
        };
    }
    
}


#endif
