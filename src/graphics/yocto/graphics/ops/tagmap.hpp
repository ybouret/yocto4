#ifndef YOCTO_GRAPHICS_OPS_TAGMAP_INCLUDED
#define YOCTO_GRAPHICS_OPS_TAGMAP_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
    namespace graphics
    {

        //! class to build a map of tags associated to an image
        class tagmap : public pixmap<size_t>
        {
        public:
            explicit tagmap(const unit_t W, const unit_t H);
            virtual ~tagmap() throw();
            size_t        current;
            stack<vertex> v_stack;

            template <typename T>
            void build(const pixmap<T> &src,
                       const size_t     links)
            {
                assert(w==src.w);
                assert(h==src.h);
                assert(links==4||links==8);

                //______________________________________________________________
                //
                //
                // initialize
                //
                //______________________________________________________________
                pixmap<size_t> &self = *this;
                ldz();             // clear pixmap
                current = 0;       // blob counter/id

                //______________________________________________________________
                //
                //
                // build tag map using a stack of contiguous vertices
                //
                //______________________________________________________________
                for(unit_t j=0;j<h;++j)
                {
                    row &                          B_j = self[j];
                    const typename pixmap<T>::row &S_j = src[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        //______________________________________________________
                        //
                        // study where we are
                        //______________________________________________________
                        size_t &B = B_j[i];
                        if( (B_j[i]<=0) && (!is_zero_pixel(S_j[i])) )
                        {
                            //__________________________________________________
                            //
                            // start a new tag
                            //__________________________________________________
                            B = ++current;

                            //__________________________________________________
                            //
                            // initialize stack with valid neighbors
                            //__________________________________________________
                            v_stack.free();
                            {
                                const vertex v(i,j);
                                grow(v,src,links);
                            }

                            //__________________________________________________
                            //
                            // work on stack
                            //__________________________________________________
                            while(v_stack.size()>0)
                            {
                                const vertex v = v_stack.peek();
                                v_stack.pop();
                                grow(v,src,links);
                            }
                        }
                    }
                }
            }




        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tagmap);
            template <typename T>
            inline void grow(const vertex    &v,
                             const pixmap<T> &src,
                             const size_t     links)
            {
                pixmap<size_t> &self = *this;
                for(size_t k=0;k<links;++k)
                {
                    const vertex tmp = v + gist::delta[k];
                    if(this->has(tmp) &&        // inside working region
                       self[tmp]<=0   &&        // no other tagged zone there
                       !is_zero_pixel(src[tmp]) // a continuous pixel
                       )
                    {
                        self[tmp] = current;
                        v_stack.push(tmp);
                    }
                }
                
            }
            
        };
    }
}

#endif
