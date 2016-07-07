#include "yocto/graphics/ops/particles.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{

    namespace graphics
    {

        particle:: ~particle() throw()
        {
        }

        particle:: particle(const size_t t) throw() :
        vnode_list(),
        tag(t),
        inside(),
        border()
        {
        }

        particle:: particle(const particle &other) :
        vnode_list(other),
        tag(other.tag),
        inside(other.inside),
        border(other.border)
        {
        }

        vertex particle:: compute_extension() throw()
        {
            vertex v;
            if(size>0)
            {
                vertex vmin = head->vtx;
                vertex vmax = head->vtx;
                for(const vnode_type *node=head->next;node;node=node->next)
                {
                    const vertex tmp = node->vtx;
                    vmin.x = min_of(vmin.x,tmp.x);
                    vmin.y = min_of(vmin.y,tmp.y);

                    vmax.x = max_of(vmax.x,tmp.x);
                    vmax.y = max_of(vmax.y,tmp.y);
                }
                v=vmax-vmin;
            }
            return v;
        }

        void particle:: regroup() throw()
        {
            merge_back(inside);
            merge_back(border);
        }


        void particle:: split_using(const tagmap &tmap) throw()
        {
            const size_t links=8;
            regroup();
            while(size>0)
            {
                vnode_type  *node = pop_back();
                const vertex vorg = node->vtx;
                size_t       count = 0;
                for(size_t i=0;i<links;++i)
                {
                    vertex   probe = vorg + gist::delta[i];
                    if(tmap.has(probe))
                    {
                        const size_t value = tmap[probe];
                        if(value==tag)
                        {
                            ++count;
                        }
                    }
                }
                if(count>=links)
                {
                    inside.push_back(node);
                }
                else
                {
                    border.push_back(node);
                }
            }
        }

        size_t particle:: dilate_with( tagmap &tmap ) throw()
        {
            if(size>0)
            {
                split_using(tmap);
            }
            const size_t links=8;
            for(const vnode_type *node=border.head;node;node=node->next)
            {
                const vertex vorg = node->vtx;
                for(size_t i=0;i<links;++i)
                {
                    vertex   probe = vorg + gist::delta[i];
                    if(tmap.has(probe))
                    {
                        size_t &value = tmap[probe];
                        if(0==value)
                        {
                            push_back( new vnode_type(probe) );
                            value = tag;
                        }
                    }
                }
            }
            std::cerr << "#new point from dilate=" << size << std::endl;
            const size_t extra = size;
            regroup();
            return extra;
        }


        bool particle:: touches( const particle &other ) const throw()
        {
            assert(0==size);
            assert(0==other.size);
            const vnode_type *lhs_start = this->border.head;
            const vnode_type *rhs_start = other.border.head;
            for(const vnode_type *lhs = lhs_start; lhs; lhs=lhs->next )
            {
                for(const vnode_type *rhs=rhs_start; rhs; rhs=rhs->next)
                {
                    if(lhs->vtx==rhs->vtx)
                    {
                        return true;
                    }
                }
            }

            return false;
        }


    }

}


#include "yocto/sort/quick.hpp"
#include "yocto/associative/map.hpp"
namespace yocto
{
    namespace graphics
    {
        particles:: ~particles() throw()
        {
        }

        particles:: particles() throw() : _particles()
        {
        }

        static inline int __compare_particle(const particle::ptr &lhs,
                                             const particle::ptr &rhs) throw()
        {
            return int(rhs->size)-int(lhs->size);
        }

        void particles:: sort() throw()
        {
            quicksort(*this,__compare_particle);
        }


        void particles:: load( const tagmap &tmap )
        {
            free();
            typedef map<size_t,particle::ptr> PDB_TYPE;
            PDB_TYPE     pdb(tmap.current,as_capacity);
            const unit_t h = tmap.h;
            const unit_t w = tmap.w;
            vertex       v;

            for(v.y=0;v.y<h;++v.y)
            {
                const tagmap::row &tmap_y = tmap[v.y];
                for(v.x=0;v.x<w;++v.x)
                {
                    const size_t   tag = tmap_y[v.x];
                    if(tag<=0) continue;

                    particle      *pa  = 0;
                    particle::ptr *ppp = pdb.search(tag);
                    if(ppp)
                    {
                        pa = (*ppp).__get();
                        assert(pa);
                        //(*ppp)->push_back( new vnode_type(v) );
                    }
                    else
                    {
                        particle::ptr pp( (pa=new particle(tag)) );
                        if( !pdb.insert(tag,pp) )
                        {
                            throw exception("unexpected multiple tag in map!");
                        }
                        assert(pa);
                    }
                    assert(pa);
                    pa->push_back( new vnode_type(v) );
                }
            }
            const size_t n = pdb.size();
            ensure(n);
            PDB_TYPE::iterator p = pdb.begin();
            for(size_t i=n;i>0;--i,++p)
            {
                push_back( *p );
            }
            sort();
        }

        void particles:: fusion( tagmap &tmap )
        {
            _particles &self = *this;
            const size_t n = 0;
            for(size_t i=1;i<=n;++i)
            {
                self[i]->dilate_with(tmap);
                self[i]->split_using(tmap);
            }
            

        }


    }
}

