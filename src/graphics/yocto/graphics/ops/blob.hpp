#ifndef YOCTO_GRAPHIX_BLOB_INCLUDED
#define YOCTO_GRAPHIX_BLOB_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/graphics/parallel.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace graphics
    {
        
        
        class blob : public pixmap<size_t>
        {
        public:
            explicit blob(size_t W,size_t H, threading::engine *server);
            virtual ~blob() throw();

            class patch : public graphics::patch
            {
            public:
                void  *handle;
                size_t target;
                size_t source;
                void  *output;
                
                explicit patch(const graphics::patch &p) throw();
                patch(const patch &) throw();
                virtual ~patch() throw();

                void change(lockable &) throw();

            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            typedef vector<blob::patch> patches;


            //! first pass: build possible neighbors
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

                        size_t      &B     = bj[i]; assert(0==B);
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
                            // tag another blob
                            B = ++counter;
                        }
                    }
                }
                //std::cerr << "counter=" << counter << std::endl;
            }

            //! second pass: reduce blobs by merging
            void __reduce(const size_t links, threading::engine *server) throw();
            
            //! fill in the sizes and renumber the blobs
            /**
             assume __reduce was called
             */
            void __format(vector<size_t>    &blobs,
                          threading::engine *server,
                          const size_t       cutoff = 0);

            
            template <typename T>
            inline void build(vector<size_t>    &sizes,
                              const pixmap<T>   &pxm,
                              const size_t       links,
                              threading::engine *server,
                              const size_t       cutoff = 0 )
            {
                __detect(pxm,links);
                __reduce(links, server);
                __format(sizes, server);
            }
            
            inline size_t __counter() const throw() { return counter; }
            
            template <typename T>
            inline void transfer(size_t     indx,
                                 pixmap<T> &pxm,
                                 const  T   value) const
            {
                assert(w==pxm.w);
                assert(h==pxm.h);
                const blob &self = *this;
                for(unit_t j=0;j<h;++j)
                {
                    const pixmap<size_t>::row &bj = self[j];
                    typename pixmap<T>::row   &pj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        T &tgt = pj[i];
                        if(bj[i]==indx)
                        {
                            tgt = value;
                        }
                        else
                        {
                            bzset(tgt);
                        }
                    }
                }
            }

            template <typename T>
            inline void transfer(size_t           indx,
                                 pixmap<T>       &pxm,
                                 const pixmap<T> &src) const
            {
                assert(w==pxm.w);
                assert(h==pxm.h);
                assert(w==src.w);
                assert(h==src.h);
                const blob &self = *this;
                for(unit_t j=0;j<h;++j)
                {
                    const pixmap<size_t>::row &bj = self[j];
                    typename pixmap<T>::row   &pj = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        T &tgt = pj[i];
                        if(bj[i]==indx)
                        {
                            memcpy(&tgt,&src[j][i], sizeof(T));
                        }
                        else
                        {
                            bzset(tgt);
                        }
                    }
                }
            }





        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            size_t          counter;
            const vertex    delta[8];
            patches         bp;
            void change_to(const type target, const type source, const graphics::patch &area) throw();
            void change_to(const type target, const type source, threading::engine *server);
        };

    }

}

#endif

