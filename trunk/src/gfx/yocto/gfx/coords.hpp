#ifndef YOCTO_GFX_COORDS_INCLUDED
#define YOCTO_GFX_COORDS_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/gfx/types.hpp"
#include "yocto/core/list.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/merge.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <typename T>
        bool is_zero_pixel(const T &) throw();
        
        template <>
        inline bool is_zero_pixel<uint8_t>(const uint8_t &u) throw()
        {
            return (u <= 0);
        }
        
        template <>
        inline bool is_zero_pixel<float>(const float &u) throw()
        {
            return fabsf(u) <= 0.0f;
        }
        
        template <>
        inline bool is_zero_pixel<double>(const double &u) throw()
        {
            return fabs(u) <= 0.0;
        }
        
        template <>
        inline bool is_zero_pixel<rgb_t>(const rgb_t &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }
        
        template <>
        inline bool is_zero_pixel<rgba_t>(const rgba_t &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }
        
        
        
        
        
        class coord
        {
        public:
            const unit_t i,j;
            
            inline  coord(unit_t x,unit_t y) throw() : i(x), j(y) {}
            inline ~coord() throw() {}
            inline  coord(const coord &other) throw() : i(other.i), j(other.j) {}
            
            inline bool is_adjacent( const coord &other ) const throw()
            {
                return (abs(i-other.i) <= 1) && (abs(j-other.j) <= 1);
            }
            
            
            class comparator
            {
            public:
                inline  comparator() throw() {}
                inline ~comparator() throw() {}
                
                inline int operator()( const coord &lhs, const coord &rhs ) throw()
                {
                    if(lhs.i<rhs.i)
                    {
                        return -1;
                    }
                    else
                    {
                        if(rhs.i<lhs.i)
                        {
                            return 1;
                        }
                        else
                        {
                            assert(lhs.i==rhs.i);
                            return lhs.j - rhs.j;
                        }
                    }
                }
            };
            
            typedef sorted_vector<coord,coord::comparator> collection;
            
            class cluster : public collection
            {
            public:
                cluster *next;
                cluster *prev;
                
                bool accept( const coord &c ) const throw()
                {
                    const collection &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        const coord &mine = self[i];
                        if(c.is_adjacent(mine))
                            return true;
                    }
                    return false;
                }
                
                explicit cluster() throw() : collection(), next(0), prev(0)
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
                    cluster *cl = head;
                    for(;cl;cl=cl->next)
                    {
                        if(cl->accept(c))
                        {
                            break;
                        }
                    }
                    
                    if(!cl)
                    {
                        cl = new cluster();
                        push_back(cl);
                    }
                    
                    if(!cl->insert(c))
                    {
                        throw exception("multiple point in cluster");
                    }
                }
                
                template <typename T>
                inline void build_from( const pixmap<T> &source )
                {
                    this->clear();
                    for(unit_t j=0;j<source.h;++j)
                    {
                        const typename pixmap<T>::row &Sj  = source[j];
                        for(unit_t i=0;i<source.w;++i)
                        {
                            if(!is_zero_pixel(Sj[i]))
                            {
                                const coord c(i,j);
                                this->insert(c);
                            }
                        }
                    }
                    core::merging<cluster>::sort(*this,cluster::compare,NULL);
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
            };
            
            
            
        private:
            YOCTO_DISABLE_ASSIGN(coord);
        };
        
        
        
        
    }
    
}


#endif
