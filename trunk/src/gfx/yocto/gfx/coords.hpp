#ifndef YOCTO_GFX_COORDS_INCLUDED
#define YOCTO_GFX_COORDS_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/gfx/types.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class coord
        {
        public:
            const unit_t i,j;
            
            inline  coord(unit_t x,unit_t y) throw() : i(x), j(y) {}
            inline ~coord() throw() {}
            inline  coord(const coord &other) throw() : i(other.i), j(other.j) {}
            
            inline bool is_adjacent( const coord &other ) const throw()
            {
                if(i==other.i)
                {
                    return (other.j == j-1) || (other.j == j+1);
                }
                
                if(j==other.j)
                {
                    return (other.i == i-1) || (other.i == i+1);
                }
                
                return false;
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
            
        private:
            YOCTO_DISABLE_ASSIGN(coord);
        };
        
        typedef sorted_vector<coord,coord::comparator> coordinates;
        
        class cluster : public coordinates
        {
        public:
            cluster *next;
            cluster *prev;
            
            explicit cluster() throw() : coordinates(), next(0), prev(0)
            {
            }
            
            virtual ~cluster() throw()
            {
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cluster);
        };
        
        class clusters : public core::list_of_cpp<cluster>
        {
        public:
            explicit clusters() throw() {}
            virtual ~clusters() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
    }
    
}


#endif
