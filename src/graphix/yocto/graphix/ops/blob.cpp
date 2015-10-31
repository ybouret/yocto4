#include "yocto/graphix/ops/blob.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphix
    {

        blob:: ~blob() throw()
        {
        }

        blob::blob(size_t W,size_t H) :
        pixmap<type>(W,H),
        delta()
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



        size_t blob:: __reduce(const size_t links) throw()
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
                                    change_to(B,P,*this);
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


        void blob:: __format(vector<size_t> &sizes)
        {
            sizes.free();
            sizes.make(counter);

            blob &self = *this;
            map<size_t,size_t> db(counter,as_capacity);
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
                        }
                        assert(k>0);
                        assert(k<=counter);
                        ++sizes[k];
                    }
                }
            }

        }
        
        
        
    }
    
}
