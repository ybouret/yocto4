#ifndef YOCTO_GRAPHIX_BLOB_INCLUDED
#define YOCTO_GRAPHIX_BLOB_INCLUDED 1

#include "yocto/graphix/rawpix.hpp"
#include "yocto/graphix/parallel.hpp"

namespace yocto
{
    namespace graphix
    {
        
        
        class blob : public pixmap<size_t>
        {
        public:
            explicit blob(size_t W,size_t H, threading::engine *server);
            virtual ~blob() throw();

            class patch : public graphix::patch
            {
            public:
                void  *handle;
                size_t target;
                size_t source;

                explicit patch(const graphix::patch &p) throw();
                patch(const patch &) throw();
                virtual ~patch() throw();

                void change(lockable &) throw();

            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            typedef vector<blob::patch> patches;


            //! first pass: build neighbors
            template <typename T>
            inline void __detect( const pixmap<T> &pxm, const size_t links)
            {
                assert(4==links||8==links);
                
               
                assert(pxm.w==w);
                assert(pxm.h==h);
                counter = 0;
                ldz();
                pixmap<size_t> &self = *this;
                for(unit_t j=0;j<h;++j)
                {
                    pixmap<type>::row             &bj = self[j];
                    const typename pixmap<T>::row &pj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        
                        if(is_zero_pixel(pj[i]))
                            continue;

                        type        &B     = bj[i]; assert(0==B);
                        bool         found = false;
                        const vertex here  = vertex(i,j);
                        for(size_t k=0;k<links;++k)
                        {
                            const vertex probe = here + delta[k];
                            if(self.has(probe))
                            {
                                const size_t value = self[probe.y][probe.x];
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
                            // tag another blocb
                            B = ++counter;
                        }
                    }
                }
                std::cerr << "counter=" << counter << std::endl;
            }
            
            size_t __reduce(const size_t links, threading::engine *server) throw();
            
            //! return the number of blobs, ranked indices from 1
            /**
             assume reduce was called
             */
            void __format(vector<size_t> &sizes);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            size_t          counter;
            const vertex    delta[8];
            patches         bp;
            void change_to(const type target, const type source, const graphix::patch &area) throw();
            void change_to(const type target, const type source, threading::engine *server) throw();
        };

    }

}

#endif

