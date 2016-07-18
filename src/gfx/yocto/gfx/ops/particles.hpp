#ifndef YOCTO_GFX_OPS_PARTICLES_INCLUDED
#define YOCTO_GFX_OPS_PARTICLES_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/pixel.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
    namespace gfx
    {

        typedef pixmap<size_t> _tagmap;
        typedef stack<vertex>  v_stack;

        //! build a map of zero separated regions
        class tagmap : public _tagmap
        {
        public:
            explicit tagmap(const unit_t W, const unit_t H);
            virtual ~tagmap() throw();

            size_t  current;
            v_stack vstk;
            
            template <typename T>
            void build(const pixmap<T> &source,
                       const size_t     links)
            {
                assert(same_metrics_than(source));
                _tagmap &self = *this;

                //______________________________________________________________
                //
                // initialize all
                //______________________________________________________________
                current = 0;
                ldz();
                vstk.free();

                for(unit_t j=0;j<h;++j)
                {
                    row                           &tag_j = self[j];
                    const typename pixmap<T>::row &src_j = source[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        size_t &tag = tag_j[i];
                        if(0==tag)
                        {
                            //__________________________________________________
                            //
                            // no current tag
                            //__________________________________________________
                            if(!pixel<T>::is_zero(src_j[i]))
                            {
                                //______________________________________________
                                //
                                // start a new tag !
                                //______________________________________________
                                tag = ++current;

                                //______________________________________________
                                //
                                // initialize stack
                                //______________________________________________
                                vstk.free();
                                {
                                    const vertex here(i,j);
                                    vstk.push(here);
                                }

                                //______________________________________________
                                //
                                // not recursive build
                                //______________________________________________
                                while( vstk.size() > 0 )
                                {
                                    //__________________________________________
                                    //
                                    // remove one vertex from stack
                                    //__________________________________________
                                    const vertex here = vstk.peek();
                                    assert(self[here]==current);
                                    vstk.pop();

                                    //__________________________________________
                                    //
                                    // probe neighbors
                                    //__________________________________________
                                    for(size_t k=0;k<links;++k)
                                    {
                                        const vertex probe = here + gist::delta[k];
                                        if(source.has(probe) && 0==self[probe] && !pixel<T>::is_zero(source[probe]))
                                        {
                                            self[probe]=current;
                                            vstk.push(probe);
                                        }
                                    }
                                }


                            }
                        }
                    }
                }

                vstk.free();
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tagmap);
        };

    }
}


#endif
