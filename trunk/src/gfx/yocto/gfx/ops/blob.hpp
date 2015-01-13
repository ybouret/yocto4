#ifndef YOCTO_GFX_BLOB_INCLUDED
#define YOCTO_GFX_BLOB_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/core/list.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
    namespace gfx
    {
        typedef pixmap<size_t> blob_type;
        
        class coord
        {
        public:
            YOCTO_MAKE_OBJECT
            const unit_t x;
            const unit_t y;
            coord       *next;
            coord       *prev;
            
            coord(unit_t X,unit_t Y) throw() : x(X), y(Y), next(0), prev(0) {}
            ~coord() throw() {}
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(coord);
        };
        
        class cluster : public counted_object, public core::list_of_cpp<coord>
        {
        public:
            typedef blob_type::type word_t;
            
            const word_t uuid;
            const word_t &key() const throw() { return uuid; }
            
            explicit cluster(const word_t id) throw() : uuid(id) {}
            virtual ~cluster() throw() {}
            
            typedef intr_ptr<word_t,cluster> ptr;
            typedef set<word_t,ptr>          db;
            
            static int compare( const cluster::ptr &lhs, const cluster::ptr &rhs ) throw()
            {
                return __compare(rhs->size, lhs->size);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cluster);
        };
        
        class clusters : public cluster::db
        {
        public:
            explicit clusters() throw() : cluster::db()
            {
            }
            
            virtual ~clusters() throw()
            {
            }
            
            inline void sort()
            {
                sort_by(cluster::compare);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
        
        class blob : public blob_type
        {
        public:
            
            
            typedef blob_type::type word_t;
            word_t                  count;
            template <typename T>
            blob(const pixmap<T> &src,
                 clusters        &cls,
                 bool             detect8 = true) :
            blob_type(src.w,src.h),
            count(0)
            {
                cls.free();
                const unit_t W     = w;
                const unit_t H     = h;
                blob_type   &B     = *this;
                const unit_t probe = detect8 ? 1 : 0;
                
                // First pass: scan lines
                for(unit_t j=0,jm=-1;j<H;++j,++jm)
                {
                    const typename pixmap<T>::row &Sj = src[j];
                    row                           &Bj = B[j];
                    unit_t i=0;
                    while(i<W)
                    {
                        // find a new segment
                        if(!has_start<T>(i,W,Sj))
                        {
                            continue;
                        }
                        unit_t k=i;
                        unit_t kp=k+1;
                        while(kp<W&& !is_zero_pixel(Sj[kp]))
                        {
                            ++k;
                            ++kp;
                        }
                        
                        // process segment
                        word_t id = 0;
                        if(j>0)
                        {
                            //find parent ?
                            const unit_t lo  = max_of<unit_t>(0,i-probe);
                            const unit_t hi  = min_of<unit_t>(W,kp+probe);
                            const row   &Bjm = B[jm];
                            for(unit_t q=lo;q<hi;++q)
                            {
                                const word_t parent = Bjm[q];
                                if(parent>0)
                                {
                                    id = Bjm[q];
                                    break;
                                }
                            }
                        }
                        
                        
                        if(id<=0)
                        {
                            id = ++count;
                            const cluster::ptr cl( new cluster(id) );
                            if( !cls.insert(cl) )
                            {
                                throw exception("unexpected new cluster failure");
                            }
                        }
                        
                        assert(id>0);
                        cluster::ptr *pp = cls.search(id);
                        if(!pp)
                        {
                            throw exception("unexpected get cluster failure");
                        }
                        
                        cluster &cc = **pp;
                        for(unit_t b=i;b<=k;++b)
                        {
                            Bj[b] = id;
                            cc.push_back( new coord(b,j) );
                        }
                        
                        // done
                        i=kp;
                    }
                }
                
                // Second pass: fusion
                (void)fusion(cls,detect8);
            }
            
            
            virtual ~blob() throw()
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blob);
            template <typename T>
            inline bool has_start(unit_t                        &i,
                                  const unit_t                   W,
                                  const typename pixmap<T>::row &Sj ) const throw()
            {
                assert(i<W);
                while(i<W)
                {
                    if(!is_zero_pixel(Sj[i]))
                    {
                        return true;
                    }
                    ++i;
                }
                return false;
            }
            
            bool fusion( cluster::db &cls, bool detect8)
            {
                bool performed = false;
                const unit_t H = h;
                const unit_t W = w;
                blob        &B = *this;
                for(unit_t j=0,jm=-1,jp=1;jp<H;++j,++jm,++jp)
                {
                    row &Bj  = B[j];
                    row &Bjp = B[jp];
                    for(unit_t i=0,im=-1,ip=1;i<W;++i,++im,++ip)
                    {
                        const word_t here = Bj[i];
                        if(!here)
                            continue;
                        
                        // scan above
                        {
                            const word_t link = Bjp[i];
                            if(link>0 && link!=here)
                            {
                                replace(link,here,cls);
                                performed = true;
                            }
                        }
                        
                        if(detect8)
                        {
                            if(i>0)
                            {
                                const word_t link = Bjp[im];
                                if(link>0 && link!=here)
                                {
                                    replace(link,here,cls);
                                    performed = true;
                                }
                            }
                            
                            if(ip<W)
                            {
                                const word_t link = Bjp[ip];
                                if(link>0 && link!=here)
                                {
                                    replace(link,here,cls);
                                    performed = true;
                                }
                            }
                        }
                    }
                }
                
                return performed;
            }
            
            
            void replace( const unit_t link, const unit_t here, cluster::db &cls )
            {
                blob        &B = *this;
                
                // replace link with here
                cluster::ptr *source = cls.search(link);
                if(!source) throw exception("can't find source cluster");
                
                cluster::ptr *target = cls.search(here);
                if(!target) throw exception("can't find target cluster");
                
                cluster &src = **source;
                cluster &tgt = **target;
                
                // rewrite values
                while(src.size)
                {
                    coord *c = src.pop_back();
                    assert(link==B[c->y][c->x]);
                    B[c->y][c->x] = here;
                    tgt.push_back(c);
                }
                
                if(!cls.remove(link))
                {
                    throw exception("unexpected cluster remove failure");
                }
                --count;
            }
            
        };
    }
    
}


#endif
