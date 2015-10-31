#include "yocto/graphix/ops/blob.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace graphix
    {
        
        blob:: ~blob() throw()
        {
        }
        
        blob::blob(size_t W,size_t H, threading::engine *server) :
        pixmap<type>(W,H),
        delta(),
        bp()
        {
            vertex *v = (vertex *)delta;
            v[0] = vertex(-1,0);
            v[1] = vertex(1,0);
            v[2] = vertex(0,1);
            v[3] = vertex(1,0);
            
            v[4] = vertex(-1,1);
            v[5] = vertex(-1,-1);
            v[6] = vertex(1,1);
            v[7] = vertex(1,-1);
            
            const size_t cpus = server ? server->size : 1;
            prepare_patches(bp,cpus,*this,true);
            for(size_t i=bp.size();i>0;--i)
            {
                bp[i].handle = this;
            }
        }
        
        
        void blob:: change_to(const type target, const type source, const graphix::patch &area) throw()
        {
            //std::cerr << "change " << source << " to " << target << std::endl;
            for(unit_t j=area.lower.y;j<=area.upper.y;++j)
            {
                pixmap<type>::row &rj = (*this)[j];
                for(unit_t i=area.lower.x;i<=area.upper.x;++i)
                {
                    type &B = rj[i];
                    if(source==B) B=target;
                }
            }
        }
        
        void blob:: change_to(const type target, const type source, threading::engine *server)
        {
            if(server)
            {
                for(size_t i=bp.size();i>0;--i)
                {
                    patch &p = bp[i];
                    p.target = target;
                    p.source = source;
                    server->enqueue(&p, &patch::change);
                }
                server->flush();
            }
            else
            {
                change_to(target, source, *this);
            }
        }
        
        
        size_t blob:: __reduce(const size_t links, threading::engine *server) throw()
        {
            assert(4==links||8==links);
            blob &self = *this;
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = self[j];
                for(unit_t i=0;i<w;++i)
                {
                    type B = rj[i];
                    if(B!=0)
                    {
                        // we are in a blob
                        const vertex here(i,j);
                        for(unit_t k=0;k<links;++k)
                        {
                            const vertex probe = here + delta[k];
                            if(this->has(probe))
                            {
                                type P = self[probe.y][probe.x];
                                if( (P!=0) && (P!=B))
                                {
                                    if(P<B) cswap(P,B);
                                    assert(P>B);
                                    change_to(B,P,server);
                                    assert(counter>0);
                                    --counter;
                                }
                            }
                        }
                    }
                }
            }
            assert(counter>=0);
            return counter;
        }
        
        
        void blob:: __format(vector<size_t> &sizes, threading::engine *server, const size_t cutoff)
        {
            sizes.free();
            sizes.make(counter);
            
            blob &self = *this;
            map<size_t,size_t> db(counter,as_capacity);
            map<size_t,size_t> tag(counter,as_capacity);
            const bool cut = cutoff>0;
            
            for(unit_t j=0;j<h;++j)
            {
                pixmap<size_t>::row &rj = self[j];
                for(unit_t i=0;i<w;++i)
                {
                    const size_t B = rj[i];
                    if(B!=0)
                    {
                        size_t *pIndx = db.search(B);
                        size_t  k     = 0;
                        if(pIndx)
                        {
                            k = *pIndx;
                        }
                        else
                        {
                            k = db.size()+1;
                            if(!db.insert(B,k))
                                throw exception("blob::format unexpected database failure!");
                            if(!tag.insert(k,B))
                                throw exception("blob::format unexpected reverse database failure!");
                        }
                        assert(k>0);
                        assert(k<=counter);
                        ++sizes[k];
                    }
                }
            }
            
            vector<size_t> indx(counter);
            for(size_t i=indx.size();i>0;--i) indx[i] = i;
            co_qsort(sizes, indx, __compare_decreasing<size_t>);
            for(size_t i=1;i<=counter;++i)
            {
                const size_t  j    = indx[i];
                const size_t *pKey = tag.search(j); if(!pKey) throw exception("blob::format unexpected missing blob tag!");
                const size_t  k    = *pKey;
                std::cerr << "#" << i << " => blob#" << j << ", size=" << sizes[i] << ", tag=" << k << std::endl;
                const size_t target = (cut&&sizes[i]<=cutoff) ? 0 : i;
                change_to(target,k,server);
            }
            
            if(cut)
            {
                while( sizes.size() && sizes.back() <= cutoff )
                {
                    sizes.pop_back();
                }
            }
        }
        
        
        blob::patch:: patch(const graphix::patch &p) throw():
        graphix::patch(p),
        handle(0),
        target(0),
        source(0)
        {
            
        }
        
        blob::patch:: patch(const blob::patch &p) throw():
        graphix::patch(p),
        handle(p.handle),
        target(p.target),
        source(p.source)
        {
        }
        
        
        blob::patch:: ~patch() throw()
        {
        }
        
        void blob:: patch::change( lockable & ) throw()
        {
            assert(handle);
            static_cast<blob *>(handle)->change_to(target, source, *this);
        }
        
        
        
    }
    
}
