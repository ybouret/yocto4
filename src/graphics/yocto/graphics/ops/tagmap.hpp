#ifndef YOCTO_GRAPHICS_OPS_TAGMAP_INCLUDED
#define YOCTO_GRAPHICS_OPS_TAGMAP_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/data2rgba.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
    namespace graphics
    {

        typedef pixmap<size_t> _tagmap;
        class particle;

        //! class to build a map of tags associated to an image
        class tagmap : public _tagmap
        {
        public:
            explicit tagmap(const unit_t W, const unit_t H);
            virtual ~tagmap() throw();
            size_t                   current;
            stack<vertex>            v_stack;
            get_named_color<size_t>  to_rgba;

            template <typename T>
            void build(const pixmap<T> &src)
            {
                assert(w==src.w);
                assert(h==src.h);

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

                            (void)__build_particle(src,i,j,B);
                        }
                    }
                }
            }




        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tagmap);
            friend class particle;
            
            template <typename T>
            size_t __build_particle(const pixmap<T> &src,
                                    const unit_t     i,
                                    const unit_t     j,
                                    size_t          &B)
            {
                assert( &B == & (*this)[j][i] );
                assert(  B <= 0 );
                assert( !is_zero_pixel(src[j][i]) );

                //__________________________________________________
                //
                // start a new tag
                //__________________________________________________
                B = ++current;
                size_t count = 1;

                //__________________________________________________
                //
                // initialize stack with valid neighbors
                //__________________________________________________
                v_stack.free();
                {
                    const vertex v(i,j);
                    grow(v,src,count);
                }

                //__________________________________________________
                //
                // work on stack
                //__________________________________________________
                while(v_stack.size()>0)
                {
                    const vertex v = v_stack.peek();
                    v_stack.pop();
                    grow(v,src,count);
                }
                return count;
            }



            template <typename T>
            inline void grow(const vertex    &v,
                             const pixmap<T> &src,
                             size_t          &count)
            {
                pixmap<size_t> &self = *this;
                for(size_t k=0;k<8;++k)
                {
                    const vertex tmp = v + gist::delta[k];
                    if(this->has(tmp) &&        // inside working region
                       self[tmp]<=0   &&        // no other tagged zone there
                       !is_zero_pixel(src[tmp]) // a continuous pixel
                       )
                    {
                        self[tmp] = current;
                        v_stack.push(tmp);
                        ++count;
                    }
                }
                
            }
            
        };
    }
}

#endif
