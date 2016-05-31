#ifndef YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/rgb.hpp"
#include "yocto/stock/stack.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace graphics
    {

        //! a blob is a list of vertices
        class blob : public counted_object, public vnode_list
        {
        public:
            typedef arc_ptr<blob> ptr;

            const size_t tag;

            explicit blob(const size_t t) throw();
            virtual ~blob() throw();

            template <typename T>
            void transfer( pixmap<T> &tgt, const pixmap<T> &src) const throw()
            {
                tgt.ldz();
                for(const vnode_type *node = head; node; node=node->next)
                {
                    const vertex v = node->vtx;
                    assert(tgt.has(v));
                    assert(src.has(v));
                    tgt[v] = src[v];
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
        };
        
        
        
        class blobs : public pixmap<size_t>
        {
        public:
            explicit blobs(const unit_t W, const unit_t H);
            virtual ~blobs() throw();

            size_t                  current;
            stack<vertex>           vstk;
            vector<blob::ptr>       content;

            template <typename T>
            void build(const pixmap<T> &src,
                       const size_t     links = 4 )
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
                content.free();    // clear blobs

                //______________________________________________________________
                //
                //
                // first pass, build blob map
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
                            // start a new blob
                            //__________________________________________________
                            B = ++current;

                            //__________________________________________________
                            //
                            // initialize stack with valid neighbors
                            //__________________________________________________
                            vstk.free();
                            {
                                const vertex v(i,j);
                                grow(v,src,links);
                            }

                            //__________________________________________________
                            //
                            // work on stack
                            //__________________________________________________
                            while(vstk.size()>0)
                            {
                                const vertex v = vstk.peek();
                                vstk.pop();
                                grow(v,src,links);
                            }
                        }
                    }
                }


                //______________________________________________________________
                //
                //
                // build contents: tagged blobs, by decreasing size
                //
                //______________________________________________________________
                setup();
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

            // build content
            void setup();
            
        };

    }

}

#endif

