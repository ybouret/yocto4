#ifndef YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/rgb.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
    namespace graphics
    {

        class blob : public object, vnode_list
        {
        public:
            blob *next;
            blob *prev;
            explicit blob() throw() : next(0), prev(0) {}
            virtual ~blob() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
        };
        
        
        
        class blobs : public pixmap<size_t>, core::list_of_cpp<blob>
        {
        public:
            explicit blobs(const unit_t W, const unit_t H);
            virtual ~blobs() throw();

            size_t                  current;
            stack<vertex>           vstk;

            template <typename T>
            void build(const pixmap<T> &src,
                       const size_t     links = 4 )
            {
                pixmap<size_t> &self = *this;
                assert(w==src.w);
                assert(h==src.h);
                assert(links==4||links==8);
                ldz();
                clear();
                current = 0;
                
                // first pass, build blob map
                for(unit_t j=0;j<h;++j)
                {
                    row &                          B_j = self[j];
                    const typename pixmap<T>::row &S_j = src[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        // study where we are
                        size_t &B = B_j[i];
                        if( (B_j[i]<=0) && (!is_zero_pixel(S_j[i])) )
                        {
                            // start a new blob
                            B = ++current;
                            
                            //initialize stack
                            vstk.free();
                            {
                                const vertex v(i,j);
                                grow(v,src,links);
                            }
                            
                            // work on stack
                            while(vstk.size()>0)
                            {
                                const vertex v = vstk.peek();
                                vstk.pop();
                                grow(v,src,links);
                            }
                        }
                    }
                }
                
                //second pass: build blobs
                

            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blobs);
            template <typename T>
            void grow( const vertex &v, const pixmap<T> &src, const size_t links)
            {
                pixmap<size_t> &self = *this;
                for(size_t k=0;k<links;++k)
                {
                    const vertex tmp = v + gist::delta[k];
                    if(this->has(tmp) &&        // inside working region
                       self[tmp]<=0   &&        // no other blob there
                       !is_zero_pixel(src[tmp]) // a continuous pixel
                       )
                    {
                        self[tmp] = current;
                        vstk.push(tmp);
                    }
                }

            }
        };

    }

}

#endif

