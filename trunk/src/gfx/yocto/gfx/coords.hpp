#ifndef YOCTO_GFX_COORDS_INCLUDED
#define YOCTO_GFX_COORDS_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/gfx/types.hpp"
#include "yocto/core/list.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
    namespace gfx
    {
        
               
        
#if 0
        
        class coord
        {
        public:
            const unit_t x,y;
            
            inline  coord(const unit_t ax, const unit_t ay) throw() : x(ax), y(ay) {}
            inline ~coord() throw() {}
            inline  coord(const coord &other) throw() : x(other.x), y(other.y) {}
            
            inline bool is_adjacent( const coord &other ) const throw()
            {
                const unit_t dx = x - other.x;
                const unit_t dy = y - other.y;
                return (abs(dx)<=1) && (abs(dy)<=1);
            }
            
            
            class cluster : public vector<coord>
            {
            public:
                cluster *next;
                cluster *prev;
                
                inline bool accept( const coord &c ) const throw()
                {
                    const array<coord> &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        const coord &mine = self[i];
                        if(c.is_adjacent(mine))
                        {
                            assert(mine.is_adjacent(c));
                            return true;
                        }
                    }
                    
                    return false;
                }
                
                explicit cluster() throw() : vector<coord>(), next(0), prev(0)
                {
                }
                
                virtual ~cluster() throw()
                {
                }
                
                static inline int compare(const cluster *lhs, const cluster *rhs, void *) throw()
                {
                    return __compare(rhs->size(), lhs->size());
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(cluster);
            };
            
            class clusters : public core::list_of_cpp<cluster>
            {
            public:
                explicit clusters() throw() {}
                virtual ~clusters() throw() {}
                
                inline void insert( const coord &c )
                {
                    cluster *cl = find_cluster_for(c);
                    
                    if(!cl)
                    {
                        cl = new cluster();
                        push_back(cl);
                    }
                    
                    cl->push_back(c);
                }
                
                
                template <typename T>
                inline void build_from( const pixmap<T> &source )
                {
                    clear();
                    for(unit_t j=0;j<source.h;++j)
                    {
                        const typename pixmap<T>::row &Sj  = source[j];
                        for(unit_t i=0;i<source.w;++i)
                        {
                            const T &value = Sj[i];
                            if(!is_zero_pixel(value))
                            {
                                const coord c(i,j);
                                insert(c);
                            }
                        }
                    }
                    while(merge_clusters())
                        ;
                    core::merging<cluster>::sort(*this,cluster::compare,NULL);
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
                
                inline cluster *find_cluster_for( const coord &c ) throw()
                {
                    for(cluster *cl = head; cl; cl=cl->next )
                    {
                        if(cl->accept(c))
                        {
                            return cl;
                        }
                    }
                    return NULL;
                }
                
                bool merge_clusters()
                {
                    bool ans = false;
                    if(size>1)
                    {
                        list_of<cluster> stk;
                        stk.push_back(pop_back());
                        while(size>0)
                        {
                            cluster *cl = pop_back();
                            assert(cl->size()>0);
                            for(cluster *guess=stk.head;guess;guess=guess->next)
                            {
                                if( should_merge(cl,guess) )
                                {
                                    ans  = true;
                                    const array<coord> &C = *cl;
                                    for(size_t i=C.size();i>0;--i)
                                    {
                                        guess->push_back(C[i]);
                                    }
                                    cl->free();
                                    break;
                                }
                            }
                            if(cl->size()<=0)
                            {
                                delete cl;
                            }
                            else
                            {
                                stk.push_back(cl);
                            }
                        }
                        swap_with(stk);
                    }
                    return ans;
                }
                
                bool should_merge(const cluster *a, const cluster *b) const throw()
                {
                    const array<coord> &A = *a;
                    const array<coord> &B = *b;
                    for(size_t i=A.size();i>0;--i)
                    {
                        const coord &Ai = A[i];
                        for(size_t j=B.size();j>0;--j)
                        {
                            if( B[j].is_adjacent(Ai) )
                            {
                                return true;
                            }
                        }
                    }
                    return false;
                }
            };
            
            
            
        private:
            YOCTO_DISABLE_ASSIGN(coord);
        };
#endif
        
        
        
    }
    
}


#endif
