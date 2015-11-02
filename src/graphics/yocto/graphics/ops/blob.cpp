#include "yocto/graphics/ops/blob.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sort/index.hpp"

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


        void blob:: __reduce(const size_t links, threading::engine *server) throw()
        {
            assert(4==links||8==links);
            blob &self = *this;
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = self[j];
                for(unit_t i=0;i<w;++i)
                {
                    size_t B = rj[i];
                    if(B!=0)
                    {
                        // we are in a blob
                        const vertex here(i,j);
                        for(unit_t k=0;k<links;++k)
                        {
                            const vertex probe = here + delta[k];
                            if(this->has(probe))
                            {
                                size_t P = self[probe.y][probe.x];
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
        }


        void blob:: __format(vector<size_t> &blobs, threading::engine *server, const size_t cutoff)
        {
            const bool  cut  = cutoff>0;
            blob       &self = *this;
            blobs.free();
            blobs.make(counter,0);
            //std::cerr << "counter=" << counter << std::endl; assert(counter==blobs.size());
            map<size_t,size_t> db(counter,as_capacity);
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = self[j];
                for(unit_t i=0;i<w;++i)
                {
                    const size_t  B     = rj[i];
                    if(B!=0)
                    {
                        size_t *pIndx = db.search(B);
                        if(pIndx)
                        {
                            size_t &indx = *pIndx;
                            assert(indx>0);
                            assert(indx<=counter);
                            ++blobs[indx];
                        }
                        else
                        {
                            const size_t indx = db.size() + 1;
                            if(!db.insert(B,indx)) throw exception("blob::format(unexpected database failure)");
                            assert(indx>0);
                            assert(indx<=counter);
                            ++blobs[indx];
                        }
                    }
                }
            }
            assert(counter==db.size());

            //! collect the different blobs indices
            vector<size_t> indx(counter,as_capacity);
            for( map<size_t,size_t>::iterator i=db.begin();i!=db.end();++i)
            {
                //std::cerr << "blob #" << i->key << " => " << *i << std::endl;
                indx.__push_back(i->key);
            }
            assert(counter==indx.size());

            //! cosort the sizes and the indices
            co_qsort(blobs,indx, __compare_decreasing<size_t>);
            map<size_t,size_t> tag(counter,as_capacity);

            //! compute new indices
            for(size_t i=1;i<=counter;++i)
            {
                size_t target = i;
                if(cut&&blobs[i]<=cutoff)
                    target=0;
                if(!tag.insert(indx[i], target))
                    throw exception("blob::format(unexpected tags failure)");
                //std::cerr << "blob @" << indx[i] << " => " << blobs[i] << "=> " << target << std::endl;
            }

            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = self[j];
                for(unit_t i=0;i<w;++i)
                {
                    size_t  &B = rj[i];
                    if(B!=0)
                    {
                        size_t *pNewB = tag.search(B);
                        if(!pNewB)
                            throw exception("blob::format(unexpected missing tag)");
                        //std::cerr << B << ":" << *pNewB << ".";
                        B = *pNewB;
                    }
                }

            }
            //std::cerr << std::endl;

            if(cut)
            {
                while( blobs.size() && blobs.back() <= cutoff )
                {
                    blobs.pop_back();
                }
            }
        }


        blob::patch:: patch(const graphix::patch &p) throw():
        graphix::patch(p),
        handle(0),
        target(0),
        source(0),
        output(0)
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
