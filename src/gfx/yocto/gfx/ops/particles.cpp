#include "yocto/gfx/ops/particles.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/comparator.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{

    namespace gfx
    {

        particle::  particle(const size_t id) throw() : vlist(), tag(id), inside(), border() {}
        particle:: ~particle() throw() {}

        void particle:: regroup() throw()
        {
            merge_back(inside);
            merge_back(border);
        }

        void particle:: split_using( const tagmap &tags ) throw()
        {
            regroup();
            while(size)
            {
                vnode *node = pop_back();
                assert(tags.has(node->vtx));
                assert(tag==tags[node->vtx]);
                const vertex here  = node->vtx;
                size_t       count = 0;
                for(size_t k=0;k<4;++k)
                {
                    const vertex probe = here + gist::delta[k];
                    if(tags.has(probe)&&tag==tags[probe])
                    {
                        ++count;
                    }
                }
                if(count>=4)
                {
                    inside.push_back(node);
                }
                else
                {
                    border.push_back(node);
                }
            }
        }

        
    }

}

#include "yocto/sort/remove-if.hpp"

namespace yocto {

    namespace gfx {

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


        void particles::regroup_all() throw()
        {
            _particles &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                self[i]->regroup();
            }
        }

        void particles::split_all_using(const tagmap &tags) throw()
        {
            _particles &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                self[i]->split_using(tags);
            }
        }

        static inline bool __is_shallow_particle( const particle::ptr &p ) throw()
        {
            return (p->inside.size<=0);
        }

        void particles:: remove_shallow_with(tagmap &tags) throw()
        {
            regroup_all();
            split_all_using(tags);
            _particles &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                const particle &pa = *self[i];
                if(pa.inside.size<=0)
                {
                    for(const vnode *node = pa.border.head;node;node=node->next)
                    {
                        assert(tags.has(node->vtx));
                        tags[node->vtx] = 0;
                    }
                }
            }
            remove_if(self,__is_shallow_particle);
            regroup_all();
            sort();
        }

        static inline bool __is_empty_particle( const particle::ptr &p ) throw()
        {
            return (p->size<=0);
        }
        
        void particles:: remove_empty() throw()
        {
            regroup_all();
            remove_if(*this,__is_empty_particle);
            sort();
        }
    }
    
}
