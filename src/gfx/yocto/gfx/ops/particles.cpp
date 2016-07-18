#include "yocto/gfx/ops/particles.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/comparator.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{

    namespace gfx
    {

        particle::  particle(const size_t id) throw() : vlist(), tag(id) {}
        particle:: ~particle() throw() {}



        particles:: ~particles() throw()
        {
        }

        particles:: particles() throw() : _particles() {}

        static inline int __compare_particles( const particle::ptr &lhs, const particle::ptr &rhs ) throw()
        {
            return __compare_decreasing(lhs->size,rhs->size);
        }
        
        void particles:: sort() throw()
        {
            _particles &self = *this;
            quicksort(self,__compare_particles);
        }

        tagmap:: tagmap(const unit_t W, const unit_t H) :
        _tagmap(W,H),
        current(0),
        vstk()
        {
        }


        tagmap:: ~tagmap() throw()
        {
            
        }


        void particles:: load(const tagmap &tags)
        {
            free();
            ensure(tags.current);
            const unit_t w=tags.w;
            const unit_t h=tags.h;

            typedef map<size_t,particle::ptr> PaDB;
            PaDB db(tags.current,as_capacity);

            vertex v;
            for(v.y=0;v.y<h;++v.y)
            {
                for(v.x=0;v.x<w;++v.x)
                {
                    const size_t tag = tags[v];
                    if(tag<=0) continue;
                    particle::ptr *ppPa = db.search(tag);
                    if(ppPa)
                    {
                        (**ppPa).push_back( new vnode(v) );
                    }
                    else
                    {
                        particle *pa = new particle(tag);
                        const particle::ptr pPa(pa);
                        if(!db.insert(tag,pPa))
                        {
                            throw exception("unexpected particle insertion failure!");
                        }
                        pa->push_back( new vnode(v) );
                    }
                }
            }
            assert(db.size()==tags.current);

            for( PaDB::iterator i=db.begin();i!=db.end();++i)
            {
                push_back(*i);
            }
            sort();
            assert(size()==tags.current);
        }

    }

}
