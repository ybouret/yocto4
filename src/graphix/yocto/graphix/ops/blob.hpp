#ifndef YOCTO_GRAPHIX_BLOB_INCLUDED
#define YOCTO_GRAPHIX_BLOB_INCLUDED 1

#include "yocto/graphix/rawpix.hpp"

namespace yocto
{
    namespace graphix
    {
        
        
        class blob : public pixmap<int>
        {
        public:
            explicit blob(size_t W,size_t H);
            virtual ~blob() throw();

            template <typename T>
            inline void detect( const pixmap<T> &pxm, const size_t links)
            {
                assert(4==links||8==links);
                
               
                assert(pxm.w==w);
                assert(pxm.h==h);
                counter = 0;
                ldz();
                pixmap<type> &self = *this;
                for(unit_t j=0;j<h;++j)
                {
                    pixmap<type>::row             &bj = self[j];
                    const typename pixmap<T>::row &pj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        if(is_zero_pixel(pj[i]))
                            continue;

                        type &B    = bj[i]; assert(0==B);
                        bool found = false;
                        const vertex here(i,j);
                        for(size_t k=0;k<links;++k)
                        {
                            const vertex probe = here + delta[k];
                            if(self.has(probe))
                            {
                                const type value = self[probe.y][probe.x];
                                if(value)
                                {
                                    B     = value;
                                    found = true;
                                    break;
                                }
                            }
                        }
                        
                        if(!found)
                        {
                            B = ++counter;
                        }
                    }
                }
                std::cerr << "counter=" << counter << std::endl;
            }
            
            void reduce(const size_t links) throw();
            
            //! return the number of blobs, ranked indices from 1
            /**
             assume reduce was called
             */
            size_t format() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            type         counter;
            const vertex delta[8];
            void change(const type target, const type source) throw();
            
        };

    }

}

#endif

