#include "yocto/graphics/ops/particles.hpp"


namespace yocto
{

    namespace graphics
    {

        particle:: ~particle() throw()
        {
        }

        particle:: particle(const size_t t) throw() :
        vnode_list(),
        tag(t)
        {
        }

        particle:: particle(const particle &other) :
        vnode_list(other),
        tag(other.tag)
        {
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

    }
}

