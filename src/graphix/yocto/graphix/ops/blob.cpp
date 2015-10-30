#include "yocto/graphix/ops/blob.hpp"
#include "yocto/associative/map.hpp"

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
        
        
        void blob:: change(const type target, const type source) throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = (*this)[j];
                for(unit_t i=0;i<w;++i)
                {
                    type &B = rj[i];
                    if(source==B) B=target;
                }
            }
        }
        
        void blob:: reduce(const size_t links) throw()
        {
            assert(4==links||8==links);
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = (*this)[j];
                for(unit_t i=0;i<w;++i)
                {
                    const type B = rj[i];
                    if(B!=0)
                    {
                        const vertex here(i,j);
                        for(unit_t k=0;k<links;++k)
                        {
                            const vertex probe = here + delta[k];
                            if(this->has(probe))
                            {
                                const type P = (*this)[probe.y][probe.x];
                                if(P&&P!=B)
                                {
                                    change(B,P);
                                    --counter;
                                }
                            }
                        }
                    }
                }
            }
            std::cerr << "counter=" << counter << std::endl;

        }
        
        
        size_t blob:: format() throw()
        {
            
            map<type,size_t> db;
            for(unit_t j=0;j<h;++j)
            {
                pixmap<type>::row &rj = (*this)[j];
                for(unit_t i=0;i<w;++i)
                {
                    const   type B = rj[i];
                    size_t *pCount = db.search(B);
                }
            }
            
            return 0;
        }

        
        
    }
    
}
