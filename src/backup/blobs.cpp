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


        blob:: blob(const blob &other) :
        vnode_list(other),
        tag(other.tag)
        {
        }

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

#if 0
        static inline bool is_in_list( const vnode_list &L, const vertex &v )
        {
            for(const vnode_type *node=L.head;node;node=node->next)
            {
                if(v==node->vtx) return true;
            }
            return false;
        }

        void blob::dilate(const unit_t w, const unit_t h, const size_t links)
        {
            assert(4==links||8==links);
            vnode_list acc;

            // accumulate single new points
            for(const vnode_type *node = head; node; node=node->next )
            {
                const vertex v = node->vtx;
                for(size_t k=0;k<links;++k)
                {
                    const vertex tmp = v + gist::delta[k];
                    if(tmp.x>=0&&tmp.x<w&&tmp.y>=0&&tmp.y<h)
                    {
                        if( !is_in_list(acc,tmp) && !is_in_list(*this,tmp) )
                        {
                            acc.push_back( new vnode_type(tmp) );
                        }
                    }
                }
            }

            while(acc.size>0)
            {
                vnode_type *node = acc.pop_back();
                assert( !is_in_list(*this,node->vtx) );
                push_back( node );
            }

        }
#endif

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
