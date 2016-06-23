#include "yocto/graphics/ops/blobs.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {
        blob:: ~blob() throw()
        {
        }

        blob:: blob(const size_t t) throw() : tag(t) {}


        vertex blob:: extension() const throw()
        {
            if(size>0)
            {
                const vnode_type *node = head;
                vertex vmin = node->vtx;
                vertex vmax = node->vtx;
                for(node=node->next;node;node=node->next)
                {
                    const vertex tmp = node->vtx;
                    vmin.x = min_of(vmin.x,tmp.x);
                    vmax.x = max_of(vmax.x,tmp.x);
                    vmin.y = min_of(vmin.y,tmp.y);
                    vmax.y = max_of(vmax.y,tmp.y);
                }
                vmax -= vmin;
                ++vmax.x;
                ++vmax.y;
                return vmax;
            }
            else
            {
                return vertex(0,0);
            }
        }

    }
}

namespace yocto
{
    namespace graphics
    {
        blobs:: ~blobs() throw()
        {
        }

        blobs:: blobs(const unit_t W, const unit_t H) :
        pixmap<size_t>(W,H),
        current(0),
        vstk(),
        content()
        {

        }


        namespace {

            static inline int compare_blobs_size(const blob::ptr &lhs,
                                                 const blob::ptr &rhs) throw()
            {
                const ptrdiff_t L(lhs->size);
                const ptrdiff_t R(rhs->size);
                return R-L;
            }

        }

        void blobs:: setup()
        {
            assert(0==content.size());
            content.ensure(current);

            //__________________________________________________________________
            //
            // dispatch the blobs into one map
            //__________________________________________________________________
            typedef map<size_t,blob::ptr> bmap_type;
            bmap_type bmap(current,as_capacity);
            vertex v;
            for(v.y=0;v.y<h;++v.y)
            {
                const row &B_y = (*this)[v.y];
                for(v.x=0;v.x<w;++v.x)
                {
                    const size_t id = B_y[v.x];
                    if(id>0)
                    {
                        blob::ptr *ppBlob = bmap.search(id);
                        if(ppBlob)
                        {
                            (*ppBlob)->push_back( new vnode_type(v) );
                        }
                        else
                        {
                            blob::ptr b( new blob(id) );
                            b->push_back( new vnode_type(v) );
                            if(!bmap.insert(id,b))
                            {
                                throw exception("unexpected insertion failure for blob#%u", unsigned(id) );
                            }
                        }
                    }
                }
            }
            assert(bmap.size()==current);

            //__________________________________________________________________
            //
            // get back into content
            //__________________________________________________________________

            for(bmap_type::iterator i=bmap.begin();i!=bmap.end();++i)
            {
                const blob::ptr &b = *i;
                content.push_back(b);
            }
            
            // decreasing order
            quicksort(content,compare_blobs_size);
        }
    }
}
