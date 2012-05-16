#ifndef YOCTO_SWAMP_DELAUNAY_INCLUDED
#define YOCTO_SWAMP_DELAUNAY_INCLUDED 1


#include "yocto/swamp/in2d.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        class iEdge
        {
        public:
            const size_t p0, p1;
            iEdge( size_t a, size_t b ) throw();
            iEdge( const iEdge &other ) throw();
            ~iEdge() throw();
            
            friend bool operator==( const iEdge &, const iEdge & ) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(iEdge);
        };
        
        class iTriangle 
        {
        public:
            iTriangle( size_t a, size_t b, size_t c ) throw();
            iTriangle( const iTriangle &other ) throw();
            virtual ~iTriangle() throw();
            
            const size_t i0, i1, i2;
            const iEdge  e0, e1, e2;
            
            
        private:
            YOCTO_DISABLE_ASSIGN(iTriangle);
        };
        
        
        template <typename T>
        class delaunay
        {
        public:
            typedef typename vertex2D<T>::type vertex;
            class triangle : public iTriangle
            {
            public:
                explicit triangle( const array<vertex> &vertices, size_t a, size_t b, size_t c );
                virtual ~triangle() throw();
                triangle( const triangle &other ) throw();
                
                triangle    *next;
                triangle    *prev;
                const vertex center;
                const T      radius;
                const T      r2;
            private:
                YOCTO_DISABLE_ASSIGN(triangle);
            };
            
            typedef core::list_of<triangle> triangles;
            
            explicit delaunay();
            virtual ~delaunay() throw();
            
            void build( const array<vertex> &vertices );
            
            const triangles & operator()(void) const throw();
            
        private:
            triangle *create( const array<vertex> &vertices, size_t a, size_t b, size_t c );
            void      destruct( triangle *tr ) throw(); //! destruct and store
            void      free() throw(); //!< remove alive triangles
            void      kill() throw(); //!< remove dead triangles
            core::pool_of<triangle> tr_pool;
            core::list_of<triangle> tr_list;
            vector<triangle*>       circum;
            
            
            void insert( const array<vertex> &vertices, size_t v_index );
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(delaunay);
            
        };
        
    }
    
}

#endif
